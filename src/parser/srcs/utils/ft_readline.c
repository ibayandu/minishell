/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:49:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 10:04:51 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

char	*ft_readline(char *prompt)
{
	char	*ret;

	ret = ft_absorb(readline(prompt));
	if (ret)
		ret = ft_strjoin(ret, "\n");
	return (ret);
}
