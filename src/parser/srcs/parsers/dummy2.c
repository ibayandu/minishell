#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isdigit
#include "your_header_with_structs.h" // Contains your typedefs and structs

// --- Assume Lexer functions and helpers are defined somewhere ---
// Updated token types
typedef enum e_token_type {
    TOKEN_WORD, TOKEN_ASSIGNMENT_WORD, TOKEN_NUMBER_WORD, // NUMBER_WORD is new
    TOKEN_AND_AND, TOKEN_OR_OR, TOKEN_GREATER_GREATER, TOKEN_LESS_LESS,
    TOKEN_PIPE, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_GT, TOKEN_LT,
    TOKEN_NEWLINE, // May or may not be used by parser depending on lexer's whitespace handling
    TOKEN_EOF, TOKEN_ERROR
} e_token_type;

typedef struct s_token {
    e_token_type type;
    char *lexeme;
    // int value; // No longer directly used by parser if NUMBER is NUMBER_WORD
} t_token;

extern t_token current_token; // Assume lexer updates this

void advance_token();
e_token_type peek_token_type();
const char* peek_token_lexeme();

// --- Memory Allocation Helpers (Same as before, ensure they are robust) ---
t_word *create_word(const char* str, int flags);
t_word_list *create_word_list_node(t_word *word);
t_word_list *append_word_list(t_word_list *head, t_word *word);
t_redirect *create_redirect_node(t_redir_type type);
t_redirect *append_redirect_list(t_redirect *head, t_redirect *new_redir);
t_command *create_command(t_cmd_type type);
// ... and freeing functions ...
void free_command(t_command *cmd); // You'll need this and other free functions

// --- Error Reporting (Same as before) ---
void parser_error(const char *message);
int consume_token(e_token_type expected_type);


// --- Helper function ---
int is_str_numeric(const char *s) {
    if (!s || *s == '\0') return 0;
    for (; *s; s++) {
        if (!isdigit((unsigned char)*s)) return 0;
    }
    return 1;
}

// --- Forward declarations of parser functions ---
t_command *parse_inputunit();
t_redirect *parse_redirection();
t_element *parse_simple_command_element();
t_redirect *parse_redirection_list();
t_simple_cmd *parse_simple_command_internal();
t_command *parse_command();
t_command *parse_list();
t_command *parse_simple_list();
t_command *parse_pipeline();


// --- Parser Function Implementations ---

t_command *parse_inputunit() {
    // inputunit: simple_list EOF
    //          | error EOF
    //          | EOF
    t_command *cmd = NULL;

    if (peek_token_type() == TOKEN_EOF) {
        return NULL; // Empty input is valid, no command
    }

    cmd = parse_simple_list();
    if (cmd) {
        if (peek_token_type() != TOKEN_EOF) { // Must end with EOF now
            parser_error("Expected EOF after command list");
            // free_command(cmd); // Clean up
            return NULL;
        }
        // Do not consume EOF, it's a marker
    } else {
        // If parse_simple_list failed, error already reported.
        // We might still be at EOF or have an error token.
        if (peek_token_type() != TOKEN_EOF) {
             // If it's not EOF after a failed parse, it means an error token might be there
             // or a token that parse_simple_list couldn't handle and didn't lead to EOF
            parser_error("Syntax error in inputunit, expecting EOF");
        }
    }
    return cmd;
}

t_redirect *parse_redirection() {
    // redirection: '>' WORD
    //            | '<' WORD
    //            | NUMBER_WORD '>' WORD
    //            | NUMBER_WORD '<' WORD
    //            | GREATER_GREATER WORD
    //            | NUMBER_WORD GREATER_GREATER WORD
    //            | LESS_LESS WORD
    //            | NUMBER_WORD LESS_LESS WORD
    t_redirect *redir = NULL;
    int redirector_fd = -1; // Default, means not specified or use standard (0 for <, 1 for >)

    // Check for optional NUMBER_WORD (redirector fd)
    if (peek_token_type() == TOKEN_NUMBER_WORD) {
        const char* fd_str = peek_token_lexeme();
        if (is_str_numeric(fd_str)) {
            redirector_fd = atoi(fd_str); // For production, use strtol with error checking
            advance_token(); // Consume NUMBER_WORD
        } else {
            // It's a NUMBER_WORD but not numeric, so it cannot be an FD redirector here.
            // This case should not happen if NUMBER_WORD is always numeric,
            // or the grammar would be ambiguous. Let's assume it's intended to be numeric.
            // If it could be non-numeric, the grammar might need adjustment or
            // this token isn't part of the redirection here.
            // For now, if it's NUMBER_WORD but not numeric, we won't consume it and
            // let the next part of the rule fail if it expects an operator.
            // Or, more strictly, error here:
            // parser_error("Expected numeric string for FD in redirection");
            // return NULL;
            // For now, let's assume if it's NUMBER_WORD here, it *must* be numeric.
        }
    }

    e_token_type op_type = peek_token_type();
    switch (op_type) {
        case TOKEN_GT:
            redir = create_redirect_node(REDIR_OUTPUT);
            advance_token(); // Consume '>'
            break;
        case TOKEN_LT:
            redir = create_redirect_node(REDIR_INPUT);
            advance_token(); // Consume '<'
            break;
        case TOKEN_GREATER_GREATER:
            redir = create_redirect_node(REDIR_APPEND);
            advance_token(); // Consume '>>'
            break;
        case TOKEN_LESS_LESS:
            redir = create_redirect_node(REDIR_UNTIL);
            advance_token(); // Consume '<<'
            break;
        default:
            if (redirector_fd != -1 && op_type != TOKEN_GT && op_type != TOKEN_LT && op_type != TOKEN_GREATER_GREATER && op_type != TOKEN_LESS_LESS) {
                // We consumed a NUMBER_WORD expecting it to be an FD, but no operator followed.
                // This is a syntax error.
                // Example: "1<" is valid, "1" by itself is not a start of redirection here if not followed by operator.
                // Backtracking the token would be complex. Usually, the parser commits.
                parser_error("Expected redirection operator after potential FD");
                // A more robust lexer/parser might "put back" the token if this was speculative.
                // For simple RD, it's an error.
            }
            return NULL; // Not a redirection (or error if FD was parsed)
    }

    // Set redirector fd
    if (redirector_fd != -1) {
        redir->redirector.fd = redirector_fd;
        redir->rflags |= 0x1; // Example flag: RFLAG_REDIRECTOR_IS_FD
    } else {
        // Default redirector (e.g., STDOUT_FILENO for >, STDIN_FILENO for <)
        // This logic might be better handled during execution or AST post-processing.
        if (redir->redir_type == REDIR_OUTPUT || redir->redir_type == REDIR_APPEND) {
            redir->redirector.fd = 1; // STDOUT
        } else if (redir->redir_type == REDIR_INPUT || redir->redir_type == REDIR_UNTIL) {
            redir->redirector.fd = 0; // STDIN
        }
         redir->rflags |= 0x1; // Still an FD, just default
    }

    // Expect WORD for filename/delimiter
    if (peek_token_type() == TOKEN_WORD) {
        redir->redirectee.filename = create_word(peek_token_lexeme(), 0);
        if (redir->redir_type == REDIR_UNTIL) {
            // strdup is important as peek_token_lexeme() buffer might change
            redir->here_doc_eof = strdup(peek_token_lexeme());
        }
        advance_token(); // Consume WORD
    } else {
        parser_error("Expected WORD (filename/delimiter) after redirection operator");
        // free(redir->here_doc_eof); // if allocated
        // free(redir->redirectee.filename); // if allocated
        // free(redir);
        return NULL;
    }

    return redir;
}

t_element *parse_simple_command_element() {
    // simple_command_element: WORD
    //                       | ASSIGNMENT_WORD
    //                       | NUMBER_WORD
    //                       | redirection
    t_element *elem = (t_element*)calloc(1, sizeof(t_element));
    if (!elem) { perror("malloc element"); exit(EXIT_FAILURE); }

    e_token_type tt = peek_token_type();

    if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD || tt == TOKEN_NUMBER_WORD) {
        int flags = 0;
        if (tt == TOKEN_ASSIGNMENT_WORD) flags = 1; // Example flag for assignment
        // For NUMBER_WORD, you might set a different flag if needed later
        elem->word = create_word(peek_token_lexeme(), flags);
        advance_token();
    } else {
        elem->redirect = parse_redirection();
        if (!elem->redirect) {
            // Not an error for parse_simple_command_element if parse_redirection returns NULL
            // because it means it wasn't a redirection.
            // However, if nothing matches, it's an error for the caller (parse_simple_command_internal).
            parser_error("Expected WORD, ASSIGNMENT_WORD, NUMBER_WORD, or redirection");
            free(elem);
            return NULL;
        }
    }
    return elem;
}

t_redirect *parse_redirection_list() {
    // redirection_list: redirection (redirection)*
    t_redirect *head = NULL;
    t_redirect *redir_node;

    // Try to parse the first redirection
    redir_node = parse_redirection();
    if (!redir_node) {
        return NULL; // No redirections found, not an error for the list itself
    }
    head = redir_node;

    // Try to parse subsequent redirections
    t_redirect *current = head;
    while ((redir_node = parse_redirection()) != NULL) {
        current->next = redir_node;
        current = current->next;
    }
    return head;
}

t_simple_cmd *parse_simple_command_internal() {
    // simple_command: simple_command_element (simple_command_element)*
    t_simple_cmd *scmd = (t_simple_cmd*)calloc(1, sizeof(t_simple_cmd));
    if (!scmd) { perror("malloc simple_cmd"); exit(EXIT_FAILURE); }

    int element_found = 0;

    // Parse first element
    t_element *elem = parse_simple_command_element();
    if (elem) {
        element_found = 1;
        if (elem->word) {
            scmd->words = append_word_list(scmd->words, elem->word);
        } else if (elem->redirect) {
            scmd->redirects = append_redirect_list(scmd->redirects, elem->redirect);
        }
        free(elem); // elem struct is temporary
    } else {
        // If the very first element fails, then it's not a simple_command.
        free(scmd);
        return NULL; // Error reported by parse_simple_command_element
    }

    // Parse subsequent elements
    while (1) {
        // Peek to see if the next token can start a simple_command_element
        e_token_type tt = peek_token_type();
        if (tt == TOKEN_WORD || tt == TOKEN_ASSIGNMENT_WORD || tt == TOKEN_NUMBER_WORD ||
            tt == TOKEN_GT || tt == TOKEN_LT || tt == TOKEN_GREATER_GREATER || tt == TOKEN_LESS_LESS)
            // Note: TOKEN_NUMBER_WORD might also start a redirection if it's numeric followed by an op.
            // parse_simple_command_element will try parse_redirection first if applicable.
        {
            elem = parse_simple_command_element();
            if (!elem) {
                // This implies an error in parsing what looked like a continuing simple command.
                // If parse_simple_command_element returned NULL, it already called parser_error.
                // free_simple_cmd(scmd); // Full cleanup needed
                free(scmd); // Simplified cleanup
                return NULL;
            }
            if (elem->word) {
                scmd->words = append_word_list(scmd->words, elem->word);
            } else if (elem->redirect) {
                scmd->redirects = append_redirect_list(scmd->redirects, elem->redirect);
            }
            free(elem);
        } else {
            break; // No more elements for this simple command
        }
    }

    if (!element_found) { // Should have been caught by the first element parse
        parser_error("Empty simple command parsed");
        // free_simple_cmd(scmd);
        free(scmd);
        return NULL;
    }
    return scmd;
}

t_command *parse_command() {
    // command: simple_command
    //        | '(' list ')'
    //        | '(' list ')' redirection_list
    t_command *cmd_node = NULL;

    if (peek_token_type() == TOKEN_LPAREN) {
        advance_token(); // Consume '('
        cmd_node = create_command(CMD_SUBSHELL);
        cmd_node->value.subshell = (t_subshell_cmd*)calloc(1, sizeof(t_subshell_cmd));
        if (!cmd_node->value.subshell) { /* error */ free(cmd_node); return NULL; }

        cmd_node->value.subshell->command = parse_list(); // Changed from compound_list
        if (!cmd_node->value.subshell->command) {
            parser_error("Expected list after '('");
            // free subshell, free cmd_node
            return NULL;
        }

        if (!consume_token(TOKEN_RPAREN)) {
            parser_error("Expected ')' after list");
            // free subshell->command, free subshell, free cmd_node
            return NULL;
        }

        // Redirections for the subshell command are attached to the t_command wrapper
        cmd_node->redirects = parse_redirection_list();
        // parse_redirection_list returns NULL if no redirections, which is fine.

    } else {
        t_simple_cmd *sc = parse_simple_command_internal();
        if (sc) {
            cmd_node = create_command(CMD_SIMPLE);
            cmd_node->value.simple = sc;
            // Redirections for simple_command are within sc->redirects
            // cmd_node->redirects should be NULL here for CMD_SIMPLE type.
        } else {
            // Error already reported by parse_simple_command_internal
            return NULL;
        }
    }
    return cmd_node;
}

t_command *parse_list() {
    // list: pipeline ( (AND_AND | OR_OR) pipeline )*
    // (Effectively, this is list: list AND_AND pipeline | list OR_OR pipeline | pipeline, rewritten)
    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND || peek_token_type() == TOKEN_OR_OR) {
        e_token_type connector_token_type = peek_token_type();
        advance_token(); // Consume AND_AND or OR_OR

        // Removed parse_newline_list()

        t_command *right_cmd = parse_pipeline(); // Grammar says 'list' here, but to avoid direct left recursion for RD, this level often parses the next higher precedence (pipeline)
                                               // If it was 'list AND_AND list', the structure is: left = pipeline, right = list.
                                               // Let's stick to the transformed: R (op R)* where R is pipeline
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in list");
            // free_command(left_cmd);
            return NULL;
        }

        t_command *conn_cmd = create_command(CMD_CONNECT);
        conn_cmd->value.connection = (t_connect_cmd*)calloc(1, sizeof(t_connect_cmd));
        if (!conn_cmd->value.connection) { /* error handling */ return NULL; }

        conn_cmd->value.connection->first = left_cmd;
        conn_cmd->value.connection->second = right_cmd;
        conn_cmd->value.connection->type = (connector_token_type == TOKEN_AND_AND) ? CNT_AND_AND : CNT_OR_OR;

        left_cmd = conn_cmd;
    }
    return left_cmd;
}

t_command *parse_simple_list() {
    // simple_list: pipeline ( (AND_AND | OR_OR) pipeline )*
    // (Effectively, this is simple_list: simple_list AND_AND pipeline | simple_list OR_OR pipeline | pipeline, rewritten)
    // This structure is identical to parse_list now.
    // If the grammar was `simple_list: simple_list AND_AND simple_list`, we parse pipeline ( (op) pipeline )*
    t_command *left_cmd = parse_pipeline();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_AND_AND || peek_token_type() == TOKEN_OR_OR) {
        e_token_type connector_token_type = peek_token_type();
        advance_token(); // Consume AND_AND or OR_OR

        // Removed parse_newline_list()

        t_command *right_cmd = parse_pipeline(); // Per left-recursion removal: R (op R)*
        if (!right_cmd) {
            parser_error("Expected pipeline after connector in simple_list");
            // free_command(left_cmd);
            return NULL;
        }

        t_command *conn_cmd = create_command(CMD_CONNECT);
        conn_cmd->value.connection = (t_connect_cmd*)calloc(1, sizeof(t_connect_cmd));
        if (!conn_cmd->value.connection) { /* error handling */ return NULL; }

        conn_cmd->value.connection->first = left_cmd;
        conn_cmd->value.connection->second = right_cmd;
        conn_cmd->value.connection->type = (connector_token_type == TOKEN_AND_AND) ? CNT_AND_AND : CNT_OR_OR;

        left_cmd = conn_cmd;
    }
    return left_cmd;
}

t_command *parse_pipeline() {
    // pipeline: command ( '|' command )*
    // (Effectively, pipeline: pipeline '|' command | command, rewritten)
    t_command *left_cmd = parse_command();
    if (!left_cmd) return NULL;

    while (peek_token_type() == TOKEN_PIPE) {
        advance_token(); // Consume '|'

        // Removed parse_newline_list()

        t_command *right_cmd = parse_command(); // Per left-recursion removal: R (op R)*
        if (!right_cmd) {
            parser_error("Expected command after '|' in pipeline");
            // free_command(left_cmd);
            return NULL;
        }

        t_command *pipe_cmd = create_command(CMD_CONNECT);
        pipe_cmd->value.connection = (t_connect_cmd*)calloc(1, sizeof(t_connect_cmd));
        if (!pipe_cmd->value.connection) { /* error handling */ return NULL; }

        pipe_cmd->value.connection->first = left_cmd;
        pipe_cmd->value.connection->second = right_cmd;
        pipe_cmd->value.connection->type = CNT_PIPE;

        left_cmd = pipe_cmd;
    }
    return left_cmd;
}

// --- Main parsing function (example) ---
/*
t_command *parse(const char* input_string) {
    // Initialize your lexer with input_string
    // setup_lexer(input_string);
    // Get the first token
    // advance_token();

    t_command *result = parse_inputunit();

    if (result && peek_token_type() != TOKEN_EOF) {
        // This can happen if parse_inputunit consumed everything it expected,
        // but there's trailing garbage not consumed by grammar rules before EOF.
        parser_error("Unexpected tokens at end of input");
        // free_command(result);
        // return NULL;
    } else if (!result && peek_token_type() != TOKEN_EOF) {
        // This means parsing failed, and we are not at EOF.
        // Error should have been reported by a sub-parser.
    }

    return result;
}
*/
