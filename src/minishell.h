/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:14:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 04:54:07 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs.h"

# define HEREDOC_MAX 16
# define PS1 "minishell> "
# define PS2 "> "

typedef struct s_minishell
{
	t_redirect	*redir_stack[HEREDOC_MAX];
	int			need_here_doc;

}	t_minishell;

#endif
