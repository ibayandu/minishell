/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:49:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 19:21:51 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "libmem.h"
#include "parser_utils.h"

char	*ft_readline(char *prompt)
{
	char	*ret;

	ret = mem_absorb(readline(prompt));
	if (ret)
		ret = ft_strjoin(ret, "\n");
	return (ret);
}
