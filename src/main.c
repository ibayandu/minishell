/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/16 19:33:38 by yzeybek          ###   ########.tr       */
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


