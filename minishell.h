/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:14:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 12:39:16 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define DEFAULT_PATH_VALUE "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."
# define HEREDOC_MAX 16
# define PS1 "minishell> " // "\\u@\\H:\\w$ "
# define PS2 "> "

char	*get_prompt();

#endif
