/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/25 14:51:45 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parsers.h"
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

void print_command(t_command *command, int cmd_count, int tab_count);

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

char	*cnt_type_to_string(t_cnt_type cnt_type)
{
	if (cnt_type == CNT_AND_AND)
		return ("and");
	else if (cnt_type == CNT_OR_OR)
		return ("or");
	else if (cnt_type == CNT_PIPE)
		return ("pipe");
	else if (cnt_type == CNT_NL)
		return ("newline");
	else
		return ("unknown");
}

void print_tab(int indent)
{
	while (indent--)
		printf("―――");
}

void	print_word(t_word *word, int tab_count)
{
	printf("Word:\n|");
	if (word)
	{
		print_tab(tab_count + 1);
		printf("word: %s\n|", word->word);
		print_tab(tab_count + 1);
		printf("flags: %i\n|", word->flags);
	}
}

void	print_redirect(t_redirect *redirect, int redir_count, int tab_count)
{
	printf("%i) Redirect:\n|", redir_count);
	if (redirect)
	{
		print_tab(tab_count + 1);
		printf("flags: %i\n|", redirect->flags);
		print_tab(tab_count + 1);
		printf("here_doc_eof: %s\n|", redirect->here_doc_eof);
		print_tab(tab_count + 1);
		print_word(redirect->redirector, tab_count + 1);
		print_tab(tab_count + 1);
		print_word(redirect->redirectee, tab_count + 1);
		print_tab(tab_count + 1);
		if (redirect->next)
			print_redirect(redirect->next, redir_count + 1, tab_count + 1);
	}
}

void	print_word_list(t_word_list *words, int tab_count)
{
	printf("Word List:\n|");
	if (words)
	{
		print_tab(tab_count + 1);
		print_word(words->word, tab_count + 1);
		print_tab(tab_count + 1);
		print_word_list(words->next, tab_count + 1);
	}
}

void	print_simple(t_simple_cmd *simple, int tab_count)
{
	printf("Simple Command:\n|");
	if (simple)
	{
		print_tab(tab_count + 1);
		printf("flags: %i\n|", simple->flags);
		print_tab(tab_count + 1);
		print_word_list(simple->words, tab_count + 1);
		print_tab(tab_count + 1);
		print_redirect(simple->redirects, 1, tab_count + 1);
	}
}

void	print_subshell(t_subshell_cmd *subshell, int cmd_count, int tab_count)
{
	printf("Subshell:\n|");
	if (subshell)
	{
		print_tab(tab_count + 1);
		printf("flags: %i\n|", subshell->flags);
		print_tab(tab_count + 1);
		print_command(subshell->command, cmd_count + 1, tab_count + 1);
	}
}

void	print_connect(t_connect_cmd *connect, int cmd_count, int tab_count)
{
	printf("Connect:\n|");
	if (connect)
	{
		print_tab(tab_count + 1);
		printf("type: %s\n|", cnt_type_to_string(connect->type));
		print_tab(tab_count + 1);
		print_command(connect->first, cmd_count + 1, tab_count + 1);
		print_tab(tab_count + 1);
		print_command(connect->second, cmd_count + 2, tab_count + 1);
	}
}

void print_command(t_command *command, int cmd_count, int tab_count)
{
	printf("%i) Command:\n|", cmd_count);
	if (command)
	{
		print_tab(tab_count + 1);
		printf("flags: %i\n|", command->flags);
		print_tab(tab_count + 1);
		printf("type: %s\n|", cmd_type_to_string(command->type));
		print_tab(tab_count + 1);
		print_redirect(command->redirects, 1, tab_count + 1);
		print_tab(tab_count + 1);
		if (command->type == CMD_CONNECT)
			print_connect(command->value.connection, cmd_count, tab_count);
		else if (command->type == CMD_SIMPLE)
			print_simple(command->value.simple, tab_count + 1);
		else if (command->type == CMD_SUBSHELL)
			print_subshell(command->value.subshell, cmd_count + 1, tab_count + 1);
		else
			printf("Invalid Type!\n|");
	}
}

#define blue "\033[0;34m"
#define yellow "\033[0;33m"
#define green "\033[0;32m"
#define red "\033[31m"
#define defaultcolor "\033[0;0m"
#include "executor.h"

int	main(int argc, char **argv, char **env)
{
	// t_token	*token;
	(void)(argc);
	(void)(argv);

	char	*input;
	// void	*err_check;

	//input = " echo */*/deneme* && ( echo \"Başlangiç: $(date)\" && echo \"Heredoc içerik\") && echo \"Alt işlem başarıli\" || echo \"Alt işlem başarısız\" | grep \"içerik\" || echo \"Eşleşme bulunamadı\" 123> output.txt 2> deneme.txt && cat < output.txt || echo \"Yeni dosyaya yazılan içerik\" | (export a=deneme && echo $a)";

	input = "cat << z";

	// char *input = "echo hello | grep h | cat > abc";
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
	execute_ast(command,&env);
	ft_free();
	return (0);
}

