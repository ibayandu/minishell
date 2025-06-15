/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:49:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 01:01:15 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "collector.h"

char	*ft_readline(char *prompt)
{
	char	*ret;

	ret = ft_absorb(readline(prompt));
	if (ret)
		add_history(ret);
	return (ret);
}
