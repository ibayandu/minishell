/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 19:05:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"
#include "init.c"
#include "lexer.h"
#include "parsers.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

void	handle_sigint(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

char	*ft_repl(t_minishell *minishell)
{
	char		*line;
	char		*ps1;
	t_variable	*v;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	ps1 = PS1;
	line = NULL;
	v = find_variable("PS1", minishell->global_variables);
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
	if (isatty(STDIN_FILENO))
		line = ft_absorb(readline(ps1));
	else
		line = ft_absorb(get_next_line(STDIN_FILENO));
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		ft_free();
		exit(0);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	if (*line)
		add_history(line);
	return (line);
}
#include <fcntl.h>
#define MAX_BUFFER_SIZE 1024

void	minirc(t_minishell *minishell)
{
	char		*home_dir;
	char		filepath[1024];
	FILE		*file;
	char		*content;
	size_t		len;
	t_command	*cmd;

	home_dir = getenv("HOME");
	if (home_dir == NULL)
	{
		fprintf(stderr, "Home directory bulunamadı.\n");
		return ;
	}
	// .minirc dosyasının tam yolunu oluşturuyoruz.
	snprintf(filepath, sizeof(filepath), "%s/.minirc", home_dir);
	// Dosyayı okuma modunda açıyoruz.
	file = fopen(filepath, "r");
	if (file == NULL)
	{
		fprintf(stderr, ".minirc dosyası bulunamadı veya açılamadı: %s\n",
			filepath);
			return ;
	}
	// Dosya içeriğini okumak için bir buffer oluşturuyoruz.
	content = (char *)malloc(MAX_BUFFER_SIZE);
	if (content == NULL)
	{
		fprintf(stderr, "Bellek ayırma hatası.\n");
		fclose(file);
		return ;
	}
	// Dosyayı okuyoruz ve içeriği 'content' değişkenine atıyoruz.
	len = fread(content, 1, MAX_BUFFER_SIZE - 1, file);
	content[len] = '\0'; // String sonlandırıcısını ekliyoruz.
	// Dosyayı kapatıyoruz.
	fclose(file);
	if (content)
	{
		if (init_lexer(ft_strjoin(content, "\n")))
		{
			cmd = parse_inputunit(minishell);
			if (minishell->need_here_doc)
				gather_here_documents(minishell);
			if (cmd)
				minishell->last_command_exit_value = execute_command(cmd,
						minishell);
		}
	}
}

int	main(void)
{
	t_minishell *const minishell = &(t_minishell){0};
	t_command	*cmd;
	char		*cmdline;
	extern char	**environ;

	initialize_shell_variables(environ, minishell);
	minirc(minishell);
	cmdline = ft_repl(minishell);
	while (cmdline)
	{
		if (init_lexer(ft_strjoin(cmdline, "\n")))
		{
			cmd = parse_inputunit(minishell);
			if (minishell->need_here_doc)
				gather_here_documents(minishell);
			if (cmd)
				minishell->last_command_exit_value = execute_command(cmd,
						minishell);
		}
		if (minishell->last_command_exit_value == 130)
			printf("\n");
		cmdline = ft_repl(minishell);
	}
	rl_clear_history();
	ft_free();
	return (minishell->last_command_exit_value);
}
