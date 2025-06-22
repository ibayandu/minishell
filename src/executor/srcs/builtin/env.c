/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:02 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 03:37:19 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_env(void)
{
	extern char	**environ;
	char		**env;

	env = environ;
	while (*env != NULL)
		printf("%s\n", *env++);
	return (0);
}
