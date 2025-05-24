include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "your_header_with_structs.h" // Contains your typedefs and structs
// #include "your_makers_header.h"
// #include "your_lexer_header.h"

// --- Assume Lexer functions and ft_ utilities are defined ---
// t_token current_token;
// void advance_token();
// e_token_type peek_token_type();
// const char* peek_token_lexeme();
// int peek_token_value();
// int consume_token(e_token_type expected_type);
// void parser_error(const char *message);

// --- Your Maker Function Declarations ---
t_command	*make_command(t_cmd_type type, t_simple_cmd *pointer);
t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type);
t_command	*make_subshell_command(t_command *command);
t_redirect	*make_redirection(t_redir source, t_redir_type redir_type, t_redir dest_and_filename, int flags);
t_word_list	*make_word_list(t_word *word, t_word_list *wlink);
t_command	*make_bare_simple_command();
t_command	*make_simple_command(t_element element, t_command *command);
t_command	*clean_simple_command(t_command *command);
t_word		*make_word(const char *string);

// External utility for reversing lists
extern t_word_list *ft_revword(t_word_list *list);
extern t_redirect *ft_revredir(t_redirect *list);
// Here doc function
extern void  make_here_document(t_redirect *temp);
// ... other ft_ functions ...


// --- Forward declarations of parser functions ---
t_command *parse_inputunit();
t_word_list *parse_word_list(); // Still here if needed by t_simple_cmd struct, though not directly in command building path
t_redirect *parse_redirection();
t_element *parse_simple_command_element();
t_redirect *parse_redirection_list();
t_command *parse_simple_command_internal();
t_command *parse_command();
t_command *parse_list();
t_command *parse_simple_list();
t_command *parse_pipeline();

// --- Main parsing function (entry point) ---
t_command *parse(const char* input_string) {
    // Initialize your lexer with input_string
    // e.g., setup_lexer(input_string);
    advance_token(); // Get the first token
    return parse_inputunit();
}

// --- Parser Functions ---

t_command *parse_inputunit() {
    // inputunit: simple_list EOF
    //          | error EOF (error recovery - typically handled by error token and recovery in parser generator)
    //          | EOF
    t_command *cmd_list = NULL;

    if (peek_token_type() == TOKEN_EOF) {
        return NULL; // Empty input
    }

    cmd_list = parse_simple_list();
    if (!cmd_list) {
        // Error should have been reported by parse_simple_list or its children.
        // We expect EOF after error or simple_list.
        if (peek_token_type() != TOKEN_EOF) {
             // This case might occur if parse_simple_list fails early AND an error token isn't explicitly handled before EOF.
             // For simplicity, we assume parse_simple_list handles its errors and we just check EOF.
             parser_error("Expected simple_list or EOF at inputunit start");
        }
        // If an error token is handled by the lexer and it advances to EOF, consume_token will catch it.
        // Or, if simple_list itself failed and didn't consume to EOF.
    }

    // Whether cmd_list is NULL (due to error) or valid, we expect EOF.
    if (!consume_token(TOKEN_EOF)) {
        // If cmd_list was valid but EOF is missing, it's an error.
        // If cmd_list was NULL due to an error, consume_token will also report an error if not EOF.
        if (cmd_list) {
            // free_command(cmd_list); // Cleanup partially parsed valid list
        }
        return NULL; // Error reported by consume_token
    }

    return cmd_list; // Can be NULL if parse_simple_list failed but was followed by EOF
}

t_word_list *parse_word_list() {
    // word_list: WORD (WORD)*
    // Not directly used in the main parsing path from simple_command,
    // but kept for completeness if the t_word_list type is needed.
    t_word_list *head = NULL;
    t_word *word_node;

    if (peek_token_type() != TOKEN_WORD) {
        // Not an error if called speculatively and no WORD is found.
        return NULL;
    }

    while (peek_token_type() == TOKEN_WORD) {
        word_node = make_word(peek_token_lexeme());
        if (!word_node) return NULL;
        head = make_word_list(word_node, head); // Prepends
        if (!head && word_node) { /* free word_node */ return NULL; }
        advance_token();
    }
    // Caller would need to ft_revword(head) if original order is needed.
    return head;
}

t_redirect *parse_redirection() {
    // redirection:	'>' WORD | '<' WORD | NUMBER '>' WORD | ...
    // (Logic remains the same as your previous version, including make_here_document call)
    t_redir source;
    t_redir dest_and_filename;
    t_redir_type redir_type_val;
    int rflags = 0;
    int source_fd = -1;

    memset(&source, 0, sizeof(t_redir));
    memset(&dest_and_filename, 0, sizeof(t_redir));

    if (peek_token_type() == TOKEN_NUMBER) {
        source_fd = peek_token_value();
        rflags = 1;
        advance_token();
    }

    e_token_type op_type = peek_token_type();
    switch (op_type) {
        case TOKEN_GT: redir_type_val = REDIR_OUTPUT; break;
        case TOKEN_LT: redir_type_val = REDIR_INPUT; break;
        case TOKEN_GREATER_GREATER: redir_type_val = REDIR_APPEND; break;
        case TOKEN_LESS_LESS: redir_type_val = REDIR_UNTIL; break;
        default:
            if (source_fd != -1) parser_error("Expected redirection operator after NUMBER");
            return NULL;
    }
    advance_token();

    if (source_fd != -1) {
        source.fd = source_fd;
    } else {
        if (redir_type_val == REDIR_OUTPUT || redir_type_val == REDIR_APPEND) source.fd = 1;
        else if (redir_type_val == REDIR_INPUT || redir_type_val == REDIR_UNTIL) source.fd = 0;
    }

    if (peek_token_type() == TOKEN_WORD || peek_token_type() == TOKEN_REDIR_WORD) {
        dest_and_filename.filename = make_word(peek_token_lexeme());
        if (!dest_and_filename.filename) return NULL;
        advance_token();
    } else {
        parser_error("Expected WORD (filename/delimiter) after redirection operator");
        return NULL;
    }

    t_redirect *redir_node = make_redirection(source, redir_type_val, dest_and_filename, rflags);
    if (!redir_node) return NULL;

    if (redir_type_val == REDIR_UNTIL) {
        make_here_document(redir_node); // Call your here-doc processing function
    }
    return redir_node;
}

t_element *parse_simple_command_element() {
    // simple_command_element: WORD | ASSIGNMENT_WORD | redirection
    // (Logic remains the same)
    t_element *elem = (t_element*)ft_malloc(sizeof(t_element));
    if (!elem) { parser_error("Malloc failed for t_element"); return NULL; }
    elem->word = NULL;
    elem->redirect = NULL;

    e_token_type tt = peek_token_type();
    if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD) {
        elem->word = make_word(peek_token_lexeme());
        if (!elem->word) { free(elem); return NULL; }
        advance_token();
    } else {
        elem->redirect = parse_redirection();
        if (!elem->redirect) {
            // This means it's not a WORD, ASSIGNMENT_WORD, or start of redirection.
            // Error reported by caller if an element was expected.
            free(elem);
            return NULL;
        }
    }
    return elem;
}

t_redirect *parse_redirection_list() {
    // redirection_list: redirection (redirection)*
    // (Logic remains the same: builds a reversed list)
    t_redirect *head = NULL;
    t_redirect *current_redir_node;

    current_redir_node = parse_redirection();
    if (!current_redir_node) {
        return NULL; // No redirections
    }
    head = current_redir_node;

    while ((current_redir_node = parse_redirection()) != NULL) {
        current_redir_node->next = head; // Prepend
        head = current_redir_node;
    }
    // Caller needs to ft_revredir(head) for original order
    return head;
}

t_command *parse_simple_command_internal() {
    // simple_command: simple_command_element (simple_command_element)*
    // (Logic remains largely the same)
    t_command *cmd = make_bare_simple_command();
    if (!cmd) return NULL;

    int element_parsed = 0;
    while (1) {
        // Peek to see if the next token can start a simple_command_element
        // This peek logic is crucial for recursive descent.
        e_token_type tt = peek_token_type();
        if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD ||
            tt == TOKEN_GT || tt == TOKEN_LT || tt == TOKEN_GREATER_GREATER || tt == TOKEN_LESS_LESS ||
            tt == TOKEN_NUMBER /* For NUMBER > ... */ || tt == TOKEN_REDIR_WORD)
        {
            t_element *elem_ptr = parse_simple_command_element();
            if (!elem_ptr) {
                // If an element was expected (first one) but failed, error.
                // If not the first, it means end of elements.
                if (!element_parsed) parser_error("Expected simple command element");
                // free_command(cmd); // If error
                return NULL; // Error or end of elements for this simple_cmd
            }
            element_parsed = 1;
            t_element temp_elem = *elem_ptr;
            free(elem_ptr);

            cmd = make_simple_command(temp_elem, cmd);
            if (!cmd) return NULL;
        } else {
            break; // No more elements for this simple command
        }
    }

    if (!element_parsed) {
        parser_error("Empty simple command parsed");
        // free_command(cmd); // (or free_bare_simple_command if it exists)
        return NULL;
    }

    cmd = clean_simple_command(cmd); // Reverses words and redirects lists
    return cmd;
}

t_command *parse_command() {
    // command: simple_command
    //        | '(' list ')'
    //        | '(' list ')' redirection_list
    t_command *cmd = NULL;

    if (peek_token_type() == TOKEN_LPAREN) {
        advance_token(); // Consume '('

        // No more newline_list handling around list for subshells
        t_command *sub_cmd_list = parse_list(); // Changed from parse_compound_list
        if (!sub_cmd_list) {
            parser_error("Expected list after '(' in subshell");
            return NULL;
        }

        cmd = make_subshell_command(sub_cmd_list);
        if (!cmd) { /* free sub_cmd_list */ return NULL; }

        if (!consume_token(TOKEN_RPAREN)) {
            parser_error("Expected ')' after list in subshell");
            // free_command(cmd);
            return NULL;
        }

        // Optional redirections for the subshell command itself
        t_redirect *redir_list = parse_redirection_list(); // Returns reversed list
        if (redir_list) {
            cmd->redirects = ft_revredir(redir_list); // Reverse to get original order
        }
    } else {
        cmd = parse_simple_command_internal();
        if (!cmd) {
            // Error already reported or it's not a command starting token
            return NULL;
        }
    }
    return cmd;
}

t_command *parse_list() {
    // list: list AND_AND list
    //     | list OR_OR list
    //     | pipeline
    // Rewritten to handle left recursion iteratively:
    // list: pipeline ( (AND_AND | OR_OR) pipeline )*

    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND || peek_token_type() == TOKEN_OR_OR) {
        e_token_type connector_token = peek_token_type();
        t_cnt_type cnt_type = (connector_token == TOKEN_AND_AND) ? CNT_AND_AND : CNT_OR_OR;
        advance_token(); // Consume AND_AND or OR_OR

        // No newline_list parsing here anymore

        t_command *right_cmd = parse_pipeline();
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in list");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, cnt_type);
        if (!left_cmd) { /* free old_left, right_cmd */ return NULL; }
    }
    return left_cmd;
}

t_command *parse_simple_list() {
    // simple_list: simple_list AND_AND simple_list
    //            | simple_list OR_OR simple_list
    //            | pipeline
    // Rewritten to handle left recursion iteratively:
    // simple_list: pipeline ( (AND_AND | OR_OR) pipeline )*
    // This function is now structurally identical to parse_list().
    // The distinction is primarily in the grammar hierarchy (inputunit -> simple_list, command -> list).

    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND || peek_token_type() == TOKEN_OR_OR) {
        e_token_type connector_token = peek_token_type();
        t_cnt_type cnt_type = (connector_token == TOKEN_AND_AND) ? CNT_AND_AND : CNT_OR_OR;
        advance_token(); // Consume AND_AND or OR_OR

        // No newline_list parsing here anymore

        t_command *right_cmd = parse_pipeline();
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in simple_list");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, cnt_type);
        if (!left_cmd) { /* free old_left, right_cmd */ return NULL; }
    }
    return left_cmd;
}

t_command *parse_pipeline() {
    // pipeline: pipeline '|' pipeline
    //         | command
    // Rewritten to handle left recursion iteratively:
    // pipeline: command ( '|' command )*

    t_command *left_cmd = parse_command();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_PIPE) {
        advance_token(); // Consume '|'

        // No newline_list parsing here anymore

        t_command *right_cmd = parse_command();
        if (!right_cmd) {
            parser_error("Expected command after '|' in pipeline");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, CNT_PIPE);
        if (!left_cmd) { /* free old_left, right_cmd */ return NULL; }
    }
    return left_cmd;
}
