/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:40 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/07 16:07:24 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command	t_command;

typedef enum e_command_type
{
	CMD_SIMPLE,
	CMD_CONNECT,
	CMD_SUBSHELL,

}	t_command_type;

typedef enum e_connect_type
{
	CNT_PIPE
}	t_connect_type;

typedef enum e_redir_type
{
	REDIR_OUTPUT
}	t_redir_type;

typedef struct	s_simple_cmd
{
	char	**args;
	t_redir	*redirects;

}	t_simple_cmd;

typedef struct s_connect_cmd
{
	t_command		*first;
	t_command		*second;
	t_connect_type	connect_type;

}	t_connect_cmd;

typedef struct s_subshell_cmd
{
	t_command	*command;
}	t_subshell_cmd;

typedef struct s_redir
{
	char	*filename;
	int		fd;

}	t_redir;


typedef struct s_redirect
{
	t_redir			redirector;
	t_redir			redirectee;
	t_redir_type	redir_type;
	struct s_redir	*next;

}	t_redirect;

typedef struct command
{
	t_command_type	type;
	t_redirect		*redirects;
	union u_content
	{
		t_connect_cmd	*connection;
		t_simple_cmd	*simple;
		t_subshell_cmd	*subshell;
	} content;

}	t_command;


#endif // PARSER_H
