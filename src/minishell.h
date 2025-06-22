/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:14:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 05:48:04 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs.h"

# define HEREDOC_MAX 16
# define PS2 "> "

typedef struct s_minishell
{
	t_redirect	*redir_stack[HEREDOC_MAX];
	int			need_here_doc;

}				t_minishell;

char			*decode_prompt(char *ps1);

#endif
