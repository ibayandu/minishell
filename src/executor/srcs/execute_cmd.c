/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:39:18 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 06:11:23 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdio.h>

int	execute_command(t_command *cmd, t_minishell *minishell)
{
	t_word_list	*result = expand_word_list(cmd->value.simple->words, 0, minishell);
	printf("%s\n", result->word->word);
	(void)result;
	return (0);
}

