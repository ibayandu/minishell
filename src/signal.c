/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:49:48 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/05 21:33:47 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "collector.h"
#include <stdlib.h>

void	handle_sigint_exec(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	ft_free();
	exit(130);
}

void	handle_sigint_main(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals_main()
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint_main);
}

void	discard_signals()
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
}

void	setup_signals_exec()
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint_exec);
}
