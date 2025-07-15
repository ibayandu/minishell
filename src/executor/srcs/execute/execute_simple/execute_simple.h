/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:47:28 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:32:25 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_SIMPLE_H
# define EXECUTE_SIMPLE_H

# include "builtin.h"
# include "execute.h"
# include "expander.h"
# include <errno.h>
# include <sys/stat.h>
# include <termios.h>

void	child_process(t_simple_cmd *cmd, t_redirect *redirects,
			t_minishell *minishell);

#endif // EXECUTE_SIMPLE_H
