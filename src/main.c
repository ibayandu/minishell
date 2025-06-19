/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/19 20:54:12 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parsers.h"
#include "printer.c"

int	main(void)
{
	t_minishell	*const	minishell = &(t_minishell){0};

	char	*input = "";
	if (!init_lexer(input))
		printf("Hata Var\n");
	t_command *command = parse_inputunit(minishell);
	gather_here_documents(minishell);
	print_command(command, 0, 1);
	ft_free();
	return (0);
}


