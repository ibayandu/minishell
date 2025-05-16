/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:40 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/16 21:00:14 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command	t_command;

typedef enum e_cmd_type
{
	CMD_SIMPLE,
	CMD_CONNECT,
	CMD_SUBSHELL

}	t_cmd_type;

typedef enum e_cnt_type
{
	CNT_PIPE,
	CNT_AND_AND,
	CNT_OR_OR

}	t_cnt_type;

typedef enum e_redir_type
{
	REDIR_OUTPUT,
	REDIR_INPUT,
	REDIR_APPEND,
	REDIR_UNTIL

}	t_redir_type;


typedef struct s_word
{
	char	*word;
	int		flags;

}	t_word;

typedef struct s_word_list
{
	t_word				*word;
	struct s_word_list	*next;

}	t_word_list;

typedef union u_redir
{
	int		fd;
	t_word	*filename;

}	t_redir;

typedef struct s_redirect
{
	int					rflags;
	int					flags;
	char				*here_doc_eof;
	t_redir				redirector;
	t_redir				redirectee;
	t_redir_type		redir_type;
	struct redirect		*next;

}	t_redirect;

typedef struct s_element
{
	t_word		*word;
	t_redirect	*redirect;

}	t_element;

typedef struct s_simple_cmd
{
	int			flags;
	t_word_list	*words;
	t_redirect	*redirects;

}	t_simple_cmd;

typedef struct s_connect_cmd
{
	t_command	*first;
	t_command	*second;
	t_cnt_type	type;

}	t_connect_cmd;

typedef struct s_subshell_cmd
{
	int			flags;
	t_command	*command;

}	t_subshell_cmd;

typedef struct s_command
{
	int				flags;
	t_cmd_type		type;
	t_redirect		*redirects;
	union u_value
	{
		t_connect_cmd	*connection;
		t_simple_cmd	*simple;
		t_subshell_cmd	*subshell;
	} value;

}	t_command;


#endif // PARSER_H
