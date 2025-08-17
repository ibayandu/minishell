/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:48:18 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 04:13:38 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "libmem.h"
#include "exec_builtin.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = ft_strdup(mem_absorb(getcwd(NULL, 0)));
	if (*cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	return (1);
}
