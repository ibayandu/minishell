#ifndef PARSER_H
# define PARSER_H

# include "../lexer/includes/token.h"
typedef enum
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_REDIRECT,
	NODE_CONTROL_FLOW,
	NODE_COMPOUND
}							ASTNodeType;

typedef struct ASTNode
{
	ASTNodeType				type;

	union
	{
		struct
		{
			char			**args;
			int				arg_count;
		} command;

		struct
		{
			int				redirect_fd_in;
			int				redirect_fd_out;
			char			*input_file;
			char			*output_file;
			int				is_here_doc;
		} redirect;

		struct
		{
			char			*condition;
			struct ASTNode	*true_branch;
			struct ASTNode	*false_branch;
		} control_flow;

		struct
		{
			char *loop_type; // while, for, etc
			char			**loop_args;
			int				loop_arg_count;
			struct ASTNode	*body;
		} loop;
	} data;

	struct ASTNode			**children;
	int						child_count;
}							ASTNode;

typedef struct
{
	t_token					*tokens;
	int						token_count;
	int						current_token;
}							Parser;

ASTNode						*parse(t_tokens *tokens);
void						print_ast(ASTNode *node, int depth);

#endif // PARSER_H