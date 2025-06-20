/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 20:22:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "structs.h"
#include <stdio.h>
int execute_command(t_command *cmd)
{
    (void)cmd;
   // t_word_list	*result = expand_word_list(cmd->value.simple->words, 0);
   // printf("%s\n", result->word->word);
   // (void)result;
    return (0);
}
