/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_panic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:51:38 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 18:35:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "parser_utils.h"

void	ft_panic(t_token *token, int *exit_code)
{
	const char	*token_value = token->value;

	if (ft_strncmp(token_value, "\n", 1) == 0)
		token_value = "newline";
	ft_putendl_fd(ft_strjoin(ft_strjoin(
				ft_strdup("minishell: syntax error near unexpected token `"),
				token_value), "'"), STDERR_FILENO);
	*exit_code = 2;
}
