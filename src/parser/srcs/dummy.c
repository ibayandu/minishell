#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "your_header_with_structs.h" // Contains your typedefs and structs
// #include "your_makers_header.h" // Should declare your maker functions
// #include "your_lexer_header.h"  // Should declare lexer types and functions

// --- Assume Lexer functions and ft_ utilities are defined ---
// t_token current_token;
// void advance_token();
// e_token_type peek_token_type();
// const char* peek_token_lexeme();
// int peek_token_value();
// int consume_token(e_token_type expected_type);
// void parser_error(const char *message);

// --- Your Maker Function Declarations (ensure these are available) ---
t_command	*make_command(t_cmd_type type, t_simple_cmd *pointer); // Generic, pointer is cast
t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type);
t_command	*make_subshell_command(t_command *command);
t_redirect	*make_redirection(t_redir source, t_redir_type redir_type, t_redir dest_and_filename, int flags);
t_word_list	*make_word_list(t_word *word, t_word_list *wlink);
t_command	*make_bare_simple_command();
t_command	*make_simple_command(t_element element, t_command *command);
t_command	*clean_simple_command(t_command *command);
t_word		*make_word(const char *string);
// t_word *alloc_word_desc(); // Internal to makers
// t_word *make_bare_word(const char *string); // Internal to makers
// t_word *make_word_flags(t_word *w, const char *string); // Internal to makers
// t_word *make_word_from_token(int token); // May not be used directly by parser for operators

// External utility for reversing lists (must be provided by you)
extern t_word_list *ft_revword(t_word_list *list);
extern t_redirect *ft_revredir(t_redirect *list);
// extern void *ft_malloc(size_t size); // Assumed used by makers
// extern char *ft_strdup(const char *s); // Assumed used by makers


// --- Forward declarations of parser functions ---
t_command *parse_inputunit();
t_word_list *parse_word_list(); // Note: This produces a list in reverse parsed order
t_redirect *parse_redirection();
t_element *parse_simple_command_element(); // Helper, allocates t_element temporarily
t_redirect *parse_redirection_list(); // Note: This produces a list in reverse parsed order
t_command *parse_simple_command_internal(); // Returns a t_command of CMD_SIMPLE type
t_command *parse_command();
t_command *parse_compound_list();
t_command *parse_list1();
int parse_list_terminator();
void parse_newline_list();
t_command *parse_simple_list1();
t_command *parse_pipeline();

// --- Parser Helper ---
// (parser_error and consume_token would be similar to the previous version)


t_command *parse_inputunit() {
    t_command *cmd = NULL;

    if (peek_token_type() == TOKEN_NEWLINE) {
        advance_token();
        return NULL; // No executable command for a blank line
    } else if (peek_token_type() == TOKEN_EOF) {
        return NULL; // No command at EOF
    }

    cmd = parse_simple_list1();
    if (cmd) {
        if (!parse_list_terminator()) {
            parser_error("Expected list_terminator (newline or EOF) after command list");
            // free_command(cmd); // Implement robust cleanup
            return NULL;
        }
        return cmd;
    }
    return NULL; // Error in parse_simple_list1 already reported
} 

t_word_list *parse_word_list() {
    // word_list: WORD (WORD)*
    // Produces a list in reverse order of parsing due to make_word_list prepending.
    // Example: input "a b c" -> list (c -> b -> a)
    t_word_list *head = NULL;
    t_word *word_node;

    if (peek_token_type() != TOKEN_WORD) {
        parser_error("Expected WORD to start word_list");
        return NULL;
    }

    while (peek_token_type() == TOKEN_WORD) {
        word_node = make_word(peek_token_lexeme());
        if (!word_node) { /* Malloc error in make_word */ return NULL; } // Propagate error
        head = make_word_list(word_node, head);
        if (!head && word_node) { /* Malloc error in make_word_list */ /* free word_node */ return NULL; }
        advance_token();
    }
    return head; // Returns list in reversed parsed order
}

t_redirect *parse_redirection() {
    t_redir source;
    t_redir dest_and_filename;
    t_redir_type redir_type_val;
    int rflags = 0; // For make_redirection's flags param; could indicate if FD was specified
    int source_fd = -1; // Default, means not specified

    // Initialize unions to avoid uninitialized reads if parts are not set
    memset(&source, 0, sizeof(t_redir));
    memset(&dest_and_filename, 0, sizeof(t_redir));


    if (peek_token_type() == TOKEN_NUMBER) {
        source_fd = peek_token_value();
        rflags = 1; // Indicate that an explicit FD was given for the redirector
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
            return NULL; // Not a redirection
    }
    advance_token(); // Consume operator

    // Set redirector
    if (source_fd != -1) {
        source.fd = source_fd;
    } else {
        // Set default FDs if not specified (your make_redirection might also do this, or executor)
        if (redir_type_val == REDIR_OUTPUT || redir_type_val == REDIR_APPEND) source.fd = 1; // STDOUT
        else if (redir_type_val == REDIR_INPUT || redir_type_val == REDIR_UNTIL) source.fd = 0; // STDIN
    }

    // Set redirectee (filename or here-doc delimiter)
    if (peek_token_type() == TOKEN_WORD || peek_token_type() == TOKEN_REDIR_WORD) {
        dest_and_filename.filename = make_word(peek_token_lexeme());
        if (!dest_and_filename.filename) { /* Malloc error */ return NULL; }
        advance_token();
    } else {
        parser_error("Expected WORD (filename/delimiter) after redirection operator");
        // Potentially free dest_and_filename.filename if partially made
        return NULL;
    }

    t_redirect *redir_node = make_redirection(source, redir_type_val, dest_and_filename, rflags);
    if (!redir_node) { /* Malloc error in make_redirection */ /* free words if necessary */ return NULL;}

    // Special handling for here_doc_eof from t_redirect struct
    if (redir_type_val == REDIR_UNTIL && dest_and_filename.filename) {
        // Assuming make_redirection doesn't automatically strdup for here_doc_eof
        // and that redirectee.filename->word is the delimiter.
        // Your make_redirection should handle this or you do it here.
        // The current make_redirection sets here_doc_eof = 0.
        // This needs clarification: does dest_and_filename.filename *become* here_doc_eof string,
        // or is redirectee a dummy for heredocs?
        // For now, let's assume here_doc_eof needs to be set separately if make_redirection doesn't.
        // If make_redirection is: temp->here_doc_eof = 0;
        // You might need:
        // if (redir_type_val == REDIR_UNTIL) {
        //     redir_node->here_doc_eof = ft_strdup(dest_and_filename.filename->word);
        //     if (!redir_node->here_doc_eof) { /* Malloc error */ /* free redir_node and words */ return NULL; }
        // }
        // This depends on your `make_redirection`'s exact behavior for REDIR_UNTIL.
        // Given `temp->here_doc_eof = 0;`, we likely need to set it.
         if (redir_type_val == REDIR_UNTIL) {
             if (redir_node->redirectee.filename && redir_node->redirectee.filename->word) {
                 redir_node->here_doc_eof = ft_strdup(redir_node->redirectee.filename->word); // Assuming ft_strdup exists
                 if (!redir_node->here_doc_eof) {
                     parser_error("Failed to allocate memory for here_doc_eof");
                     // free redir_node and its contents
                     return NULL;
                 }
             } else {
                 parser_error("Missing filename for heredoc delimiter");
                 // free redir_node
                 return NULL;
             }
         }
    }
    return redir_node;
}

t_element *parse_simple_command_element() {
    // t_element is a temporary structure for passing data to make_simple_command
    t_element *elem = (t_element*)ft_malloc(sizeof(t_element)); // Use ft_malloc if preferred for all allocs
    if (!elem) { parser_error("Malloc failed for t_element"); return NULL; }
    elem->word = NULL;
    elem->redirect = NULL;

    e_token_type tt = peek_token_type();

    if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD) {
        elem->word = make_word(peek_token_lexeme());
        // make_word sets W_HASDOLLAR, W_QUOTED.
        // If ASSIGNMENT_WORD needs a specific flag on t_word not handled by make_word, adjust here.
        // Your make_word only looks for $,'
        // If ASSIGNMENT_WORD has a distinct t_word flag, it's not set by make_word directly.
        // Assuming `make_word` is sufficient or flags are handled post-parsing.
        if (!elem->word) { /* Malloc error */ free(elem); return NULL; }
        advance_token();
    } else {
        elem->redirect = parse_redirection();
        if (!elem->redirect) {
            // Not an error if it's not a redirection, means end of simple_command_elements
            // *unless* we were expecting one.
            // The calling context (parse_simple_command_internal) checks if *any* element was parsed.
            // If parse_redirection itself had an error, it would have called parser_error.
            parser_error("Expected WORD, ASSIGNMENT_WORD, or redirection"); // This error is if nothing matches
            free(elem);
            return NULL;
        }
    }
    return elem;
}

t_redirect *parse_redirection_list() {
    // redirection_list: redirection (redirection)*
    // Builds a list of t_redirect nodes, prepending.
    // Result is in reverse order of parsing. Needs ft_revredir by caller if forward order needed.
    t_redirect *head = NULL;
    t_redirect *redir_node;

    // Try to parse the first redirection
    redir_node = parse_redirection();
    if (!redir_node) {
        return NULL; // No redirections found, not an error in itself.
    }
    head = redir_node; // First node becomes head, no next yet.

    // Parse subsequent redirections
    while ((redir_node = parse_redirection()) != NULL) {
        redir_node->next = head; // Prepend
        head = redir_node;
    }
    return head; // List is in reverse parsed order
}

t_command *parse_simple_command_internal() {
    t_command *cmd = make_bare_simple_command();
    if (!cmd) { /* Malloc error */ return NULL; }

    int element_parsed = 0;
    while (1) {
        // Peek to see if the next token can start a simple_command_element
        e_token_type tt = peek_token_type();
        if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD ||
            tt == TOKEN_GT || tt == TOKEN_LT || tt == TOKEN_GREATER_GREATER || tt == TOKEN_LESS_LESS ||
            tt == TOKEN_NUMBER /* For NUMBER > ... */ || tt == TOKEN_REDIR_WORD /* If it can start redir */)
        {
            t_element *elem_ptr = parse_simple_command_element();
            if (!elem_ptr) {
                // Error in parsing the element, or it wasn't a valid element start
                // If parse_simple_command_element returned NULL because it's not an element,
                // and we haven't parsed any elements yet, it's an error.
                // If it had an internal error, it would have called parser_error.
                if (!element_parsed) {
                     parser_error("Expected simple command element");
                } // else, we just finished the list of elements.
                // free_command(cmd); // Clean up partially built cmd
                // free(elem_ptr); // elem_ptr would be NULL if parse_simple_command_element failed to alloc
                return NULL; // Propagate error or signal end of simple_cmd
            }

            element_parsed = 1;
            // Use the content of elem_ptr, then free elem_ptr
            t_element temp_elem = *elem_ptr;
            free(elem_ptr); // Free the temporary t_element container

            cmd = make_simple_command(temp_elem, cmd);
            if (!cmd) { /* Malloc error in make_simple_command or its sub-calls */ return NULL; }
        } else {
            break; // No more tokens that can start a simple_command_element
        }
    }

    if (!element_parsed) { // A simple_command must have at least one element
        parser_error("Empty simple command");
        // free_command(cmd); // Or specialized free for bare simple command
        return NULL;
    }

    cmd = clean_simple_command(cmd); // Reverses words and redirects lists
    if (!cmd) { /* Error in clean_simple_command? Unlikely if it just reverses. */ return NULL; }
    return cmd;
}


t_command *parse_command() {
    t_command *cmd = NULL;

    if (peek_token_type() == TOKEN_LPAREN) {
        advance_token(); // Consume '('

        t_command *sub_cmd_list = parse_compound_list();
        if (!sub_cmd_list) {
            parser_error("Expected compound_list after '('");
            return NULL;
        }

        cmd = make_subshell_command(sub_cmd_list);
        if (!cmd) { /* Malloc error */ /* free sub_cmd_list */ return NULL; }

        if (!consume_token(TOKEN_RPAREN)) {
            parser_error("Expected ')' after compound_list");
            // free_command(cmd); // Full cleanup
            return NULL;
        }

        // Parse redirections for the subshell command itself
        // These are attached to the wrapping t_command (cmd->redirects)
        t_redirect *redir_list = parse_redirection_list(); // Returns list in reverse order
        if (redir_list) {
            cmd->redirects = ft_revredir(redir_list); // Reverse to get original order
            if (!cmd->redirects && redir_list) { /* ft_revredir failed or list became empty */ }
        }
        // If parse_redirection_list had an internal error, it would return NULL and report.
    } else {
        cmd = parse_simple_command_internal(); // This already returns a t_command of CMD_SIMPLE
        if (!cmd) {
            // Error already reported by parse_simple_command_internal
            return NULL;
        }
    }
    return cmd;
}

t_command *parse_compound_list() {
    parse_newline_list();
    t_command *cmd = parse_list1();
    if (!cmd) return NULL;

    if (peek_token_type() == TOKEN_NEWLINE) {
        advance_token();
        parse_newline_list();
    }
    return cmd;
}

t_command *parse_list1() {
    // list1: pipeline ( (AND_AND | OR_OR | '\n') newline_list pipeline )*
    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND ||
           peek_token_type() == TOKEN_OR_OR ||
           peek_token_type() == TOKEN_NEWLINE) {

        e_token_type connector_token = peek_token_type();
        t_cnt_type cnt_type;

        if (connector_token == TOKEN_AND_AND) cnt_type = CNT_AND_AND;
        else if (connector_token == TOKEN_OR_OR) cnt_type = CNT_OR_OR;
        else if (connector_token == TOKEN_NEWLINE) {
            // Handle '\n' as a connector if intended.
            // Your t_cnt_type doesn't have a sequence type.
            // This implies '\n' might be just a separator for a list of pipelines,
            // or it needs a CNT_SEQ. The grammar `list1: list1 X list1` suggests a binary op.
            // If it's just a separator, the grammar should be `list1: pipeline (newline_list pipeline)*`
            // and this loop structure is incorrect for that.
            // Assuming for now this means sequential execution and should be mapped if possible,
            // or error if not supported by t_cnt_type.
            parser_error("Newline as a binary connector in list1 not directly supported by t_cnt_type without CNT_SEQ or similar");
            // free_command(left_cmd);
            return NULL;
        }
        advance_token(); // Consume connector

        parse_newline_list();

        t_command *right_cmd = parse_pipeline();
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in list1");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, cnt_type);
        if (!left_cmd) { /* Malloc error in command_connect */ /* free old left_cmd, right_cmd */ return NULL; }
    }
    return left_cmd;
}

int parse_list_terminator() {
    if (peek_token_type() == TOKEN_NEWLINE) {
        advance_token();
        return 1;
    } else if (peek_token_type() == TOKEN_EOF) {
        return 1; // Don't consume EOF
    }
    return 0;
}

void parse_newline_list() {
    while (peek_token_type() == TOKEN_NEWLINE) {
        advance_token();
    }
}

t_command *parse_simple_list1() {
    // simple_list1: pipeline ( (AND_AND | OR_OR) newline_list pipeline )*
    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND || peek_token_type() == TOKEN_OR_OR) {
        e_token_type connector_token = peek_token_type();
        t_cnt_type cnt_type = (connector_token == TOKEN_AND_AND) ? CNT_AND_AND : CNT_OR_OR;
        advance_token();

        parse_newline_list();

        t_command *right_cmd = parse_pipeline();
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in simple_list1");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, cnt_type);
        if (!left_cmd) { /* Malloc error */ /* free old left_cmd, right_cmd */ return NULL; }
    }
    return left_cmd;
}

t_command *parse_pipeline() {
    // pipeline: command ( '|' newline_list command )*
    t_command *left_cmd = parse_command();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_PIPE) {
        advance_token(); // Consume '|'
        parse_newline_list();

        t_command *right_cmd = parse_command();
        if (!right_cmd) {
            parser_error("Expected command after '|' in pipeline");
            // free_command(left_cmd);
            return NULL;
        }

        left_cmd = command_connect(left_cmd, right_cmd, CNT_PIPE);
        if (!left_cmd) { /* Malloc error */ /* free old left_cmd, right_cmd */ return NULL; }
    }
    return left_cmd;
}
