/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/11 20:32:43 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>

const char	*token_type_to_string2(t_token_type type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_LESS)
		return ("T_LESS");
	else if (type == T_GREAT)
		return ("T_GREAT");
	else if (type == T_DLESS)
		return ("T_DLESS");
	else if (type == T_DGREAT)
		return ("T_DGREAT");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_AND_IF)
		return ("T_AND_IF");
	else if (type == T_OR_IF)
		return ("T_OR_IF");
	else if (type == T_LPARANTHESE)
		return ("T_LPARANTHESE");
	else if (type == T_RPARANTHESE)
		return ("T_RPARANTHESE");
	else if (type == T_EOF)
		return ("T_EOF");
	else
		return ("UNKNOWN");
}

void	print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("  ");
}

const char	*get_ast_type_name(t_ast_type type)
{
	switch (type)
	{
	case AST_COMMAD:
		return ("COMMAND");
	case AST_PIPE:
		return ("PIPE");
	case AST_SEQUENCE:
		return ("SEQUENCE");
	case AST_SUBSHELL:
		return ("SUBSHELL");
	default:
		return ("UNKNOWN_AST_TYPE");
	}
}

const char	*get_redir_type_name(t_redir_type type)
{
	switch (type)
	{
	case R_IN:
		return ("< (R_IN)");
	case R_OUT:
		return ("> (R_OUT)");
	case R_APPEND:
		return (">> (R_APPEND)");
	case R_HEREDOC:
		return ("<< (R_HEREDOC)");
	default:
		return ("UNKNOWN_REDIR_TYPE");
	}
}

void	print_redirs(t_redir *redir, int depth)
{
	while (redir)
	{
		print_indent(depth);
		printf("Redirection: %s -> %s\n", get_redir_type_name(redir->type),
			redir->file);
		redir = redir->next;
	}
}

void	print_argv(char **argv, int depth)
{
	if (!argv)
		return ;
	print_indent(depth);
	printf("Arguments:");
	for (int i = 0; argv[i]; i++)
	{
		printf(" %s", argv[i]);
	}
	printf("\n");
}

void	print_ast(t_ast *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	printf("AST Node Type: %s\n", get_ast_type_name(node->type));
	print_argv(node->argv, depth + 1);
	print_redirs(node->redirs, depth + 1);
	if (node->left)
	{
		print_indent(depth);
		printf("Left:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		print_indent(depth);
		printf("Right:\n");
		print_ast(node->right, depth + 1);
	}
}

#define blue "\033[0;34m"
#define yellow "\033[0;33m"
#define green "\033[0;32m"
#define defaultcolor "\033[0;0m"

int	main(void)
{
	t_list	*tokens;
	t_list	*current;
	t_token	*token;
	char	*input;
	t_ast	*ast;

	input = "( echo\\ \"Başlangiç: $(date)\"\
		&& cat <<EOF; echo \"Heredoc içerik\"; EOF )\
		&& echo \"Alt işlem\\\" başarıli\"\
		|| echo \"Alt işlem başarısız\" | grep \"içerik\"\
		|| echo \"Eşleşme bulunamadı\" > output.txt && cat < output.txt\
		|| cat <<EOF > new_output.txt; echo \"Yeni dosyaya yazılan içerik\"; EOF";
	tokens = lexer(input);
	current = tokens;
	while (current)
	{
		token = (t_token *)current->content;
		printf(yellow "Token Type: " blue "%-13s " yellow "Value: " green "%s" defaultcolor "\n",
			token_type_to_string2(token->token_type), token->value);
		current = current->next;
	}
	printf("deneme\n");
	ast = parser(&tokens);
	printf("deneme\n");
	printf("---%d----\n", ast->left->type);
	printf("test\n");
	ft_free();
	return (0);
}
