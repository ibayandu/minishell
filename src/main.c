/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/20 22:29:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parsers.h"
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

char *cmd_type_to_string(t_cmd_type cmd_type)
{
	if (cmd_type == CMD_SIMPLE)
		return "simple";
	else if(cmd_type == CMD_CONNECT)
		return "Connect";
	else if(cmd_type == CMD_SUBSHELL)
		return "subshell";
	else
		return "UNKNOWN";
}


void print_tab(int indent)
{
	while (indent--)
		printf("\t");
}

void	print_word(t_word *word, int tab_count)
{
	printf("Word:\n");
	print_tab(tab_count + 1);
	if (word)
	{
		printf("word: %s\n", word->word);
		print_tab(tab_count + 1);
		printf("flags: %i\n", word->flags);
	}
}

void	print_redirect(t_redirect *redirect, int redir_count, int tab_count)
{
	printf("%i) Redirect:\n", redir_count);
	print_tab(tab_count + 1);
	printf("flags: %i\n", redirect->flags);
	print_tab(tab_count + 1);
	printf("here_doc_eof: %s\n", redirect->here_doc_eof);
	print_tab(tab_count + 1);
	print_word(redirect->redirector, tab_count + 1);
	print_tab(tab_count + 1);
	print_word(redirect->redirectee, tab_count + 1);
	print_tab(tab_count + 1);
	if (redirect->next)
		print_redirect(redirect->next, redir_count + 1, tab_count + 1);
}

void print_command(t_command *command, int tab_count)
{
	int			redir_count;
	t_redirect	*temp;

	temp = NULL;
	printf("%i) Command:\n", tab_count);
	print_tab(tab_count + 1);
	printf("flags: %i\n", command->flags);
	print_tab(tab_count + 1);
	printf("type: %s\n", cmd_type_to_string(command->type));
	redir_count = -1;
	if (command->redirects)
		temp = command->redirects;
	print_tab(tab_count + 1);
	while (temp)
	{
		redir_count++;
		print_redirect(command->redirects, redir_count, tab_count + 1);
		temp = temp->next;
	}
	if (command->type == CMD_CONNECT)
		;/*print_connect(command->value.connection);*/
	else if (command->type == CMD_SIMPLE)
		;/*print_simple(command->value.simple);*/
	else if (command->type == CMD_SUBSHELL)
		;/*print_subshell(command->value);*/
	else
		printf("Invalid Type!\n");
}

#define blue "\033[0;34m"
#define yellow "\033[0;33m"
#define green "\033[0;32m"
#define red "\033[31m"
#define defaultcolor "\033[0;0m"

int	main(void)
{
	// t_token	*token;
	/*
	char	*input;
	void	*err_check;

	input = " ( echo \"Başlangiç: $(date)\"\
		&& cat <<EOF; echo \"Heredoc içerik\"; EOF )\
		&& echo \"Alt işlem başarıli\"\
		|| echo \"Alt işlem başarısız\" | grep \"içerik\"\
		|| echo \"Eşleşme bulunamadı\" > output.txt && cat < output.txt\
		|| cat <<EOF > new_output.txt; 'test' echo \"Yeni dosyaya yazılan içerik\" EOF\
		export a=deneme";
	err_check = init_lexer(input);
	if (!err_check)
		printf("Hata Var\n");
	*/


	char *input = "(ls) > deneme | (ech deneme) && echo basarili || basarisiz";
	char *err_check = init_lexer(input);
	if (!err_check)
		printf("Hata Var\n");
	/*while ((token = get_current_token()))
	{
		printf(yellow "Token Type: " blue "%-13s " yellow "Value: " green "%-35s" yellow "Token Flag" red "%d" defaultcolor "\n",
			token_type_to_string2(token->token_type), token->value,
			token->flags);
		get_next_token();
	}*/

	t_command *command = parse_inputunit();
	print_command(command, 0);
	ft_free();
	return (0);
}
