#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../lexer/includes/token_utils.h"

// Yardımcı fonksiyonlar
ASTNode* create_node(ASTNodeType type) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    return node;
}

void add_argument(ASTNode *node, const char *arg) {
    if (node->type == NODE_COMMAND) {
        node->data.command.args = realloc(
            node->data.command.args, 
            (node->data.command.arg_count + 1) * sizeof(char*)
        );
        node->data.command.args[node->data.command.arg_count] = strdup(arg);
        node->data.command.arg_count++;
    }
}

void add_child(ASTNode *parent, ASTNode *child) {
    parent->children = realloc(
        parent->children, 
        (parent->child_count + 1) * sizeof(ASTNode*)
    );
    parent->children[parent->child_count] = child;
    parent->child_count++;
}

// Token kontrol fonksiyonları
int is_word_token(t_token token) {
    return token.type == WORD;
}

int is_operator_token(t_token token) {
    return token.type == OPERATOR;
}

// Parse fonksiyonları
ASTNode* parse_command(Parser *parser);
ASTNode* parse_redirect(Parser *parser, ASTNode *cmd);
ASTNode* parse_control_flow(Parser *parser);
ASTNode* parse_loop(Parser *parser);

ASTNode* parse_command(Parser *parser) {
    ASTNode *cmd = create_node(NODE_COMMAND);
    
    while (parser->current_token < parser->token_count && 
           is_word_token(parser->tokens[parser->current_token])) {
        add_argument(cmd, parser->tokens[parser->current_token].value);
        parser->current_token++;
    }

    // Yönlendirmeleri kontrol et
    if (parser->current_token < parser->token_count && 
        is_operator_token(parser->tokens[parser->current_token])) {
        cmd = parse_redirect(parser, cmd);
    }

    return cmd;
}

ASTNode* parse_redirect(Parser *parser, ASTNode *cmd) {
    ASTNode *redirect = create_node(NODE_REDIRECT);
    
    while (parser->current_token < parser->token_count && 
           is_operator_token(parser->tokens[parser->current_token])) {
        char *op = parser->tokens[parser->current_token].value;
        parser->current_token++;

        if (strcmp(op, "<") == 0) {
            redirect->data.redirect.input_file = strdup(parser->tokens[parser->current_token].value);
            parser->current_token++;
        } else if (strcmp(op, ">") == 0) {
            redirect->data.redirect.output_file = strdup(parser->tokens[parser->current_token].value);
            parser->current_token++;
        } else if (strcmp(op, "<&") == 0) {
            redirect->data.redirect.redirect_fd_in = atoi(parser->tokens[parser->current_token].value);
            parser->current_token++;
        } else if (strcmp(op, ">&") == 0) {
            redirect->data.redirect.redirect_fd_out = atoi(parser->tokens[parser->current_token].value);
            parser->current_token++;
        }
    }

    // Komutun çocuğu olarak yönlendirmeyi ekle
    add_child(redirect, cmd);
    return redirect;
}

ASTNode* parse_control_flow(Parser *parser) {
    ASTNode *flow = create_node(NODE_CONTROL_FLOW);
    
    // if kontrolü
    if (strcmp(parser->tokens[parser->current_token].value, "if") == 0) {
        parser->current_token++;
        
        // Koşulu parse et
        flow->data.control_flow.condition = strdup(parser->tokens[parser->current_token].value);
        parser->current_token++;

        // True branch
        flow->data.control_flow.true_branch = parse_command(parser);

        // else kontrolü
        if (parser->current_token < parser->token_count && 
            strcmp(parser->tokens[parser->current_token].value, "else") == 0) {
            parser->current_token++;
            flow->data.control_flow.false_branch = parse_command(parser);
        }
    }

    return flow;
}

ASTNode* parse_loop(Parser *parser) {
    ASTNode *loop = create_node(NODE_CONTROL_FLOW);
    
    // while kontrolü
    if (strcmp(parser->tokens[parser->current_token].value, "while") == 0) {
        loop->data.loop.loop_type = strdup("while");
        parser->current_token++;

        // Döngü argümanları
        while (parser->current_token < parser->token_count && 
               is_word_token(parser->tokens[parser->current_token])) {
            add_argument(loop, parser->tokens[parser->current_token].value);
            parser->current_token++;
        }

        // Döngü gövdesi
        loop->data.loop.body = parse_command(parser);
    }

    return loop;
}

ASTNode* parse(t_tokens *tokens) {
    Parser parser = {
        .tokens = tokens->tokens,
        .token_count = tokens->count,
        .current_token = 0
    };

    ASTNode *root = create_node(NODE_COMPOUND);

    while (parser.current_token < tokens->count) {
        ASTNode *node = NULL;

        // Token tipine göre parse et
        if (is_word_token(parser.tokens[parser.current_token])) {
            if (strcmp(parser.tokens[parser.current_token].value, "if") == 0) {
                node = parse_control_flow(&parser);
            } else if (strcmp(parser.tokens[parser.current_token].value, "while") == 0) {
                node = parse_loop(&parser);
            } else {
                node = parse_command(&parser);
            }
        } else {
            parser.current_token++;
            continue;
        }

        if (node) {
            add_child(root, node);
        }
    }

    return root;
}

  

// Debug için AST yazdırma fonksiyonu
void print_ast(ASTNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");
    
    printf("Node Type: %d\n", node->type);

    if (node->type == NODE_COMMAND) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("Args: ");
        for (int j = 0; j < node->data.command.arg_count; j++) {
            printf("%s ", node->data.command.args[j]);
        }
        printf("\n");
    }

    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

// Bellek temizleme
void free_ast(ASTNode *node) {
    if (!node) return;

    // Argümanları temizle
    if (node->type == NODE_COMMAND) {
        for (int i = 0; i < node->data.command.arg_count; i++) {
            free(node->data.command.args[i]);
        }
        free(node->data.command.args);
    }

    // Alt düğümleri temizle
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }
    free(node->children);

    free(node);
}

// // Örnek kullanım
// int main() {
//     // Token dizinizi buraya ekleyin
//     // parse() ve print_ast() ile kullanabilirsiniz
//     return 0;
// }
