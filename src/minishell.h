/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:14:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/23 15:00:38 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs.h"

# define DEFAULT_PATH_VALUE "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."
# define HEREDOC_MAX 16
# define PS1 "\\u@\\H:\\w\\$ "
# define PS2 "> "

typedef struct s_variable
{
	char				*name;
	char				*value;
	char				*exportstr;
	int					attributes;
	int					context;

}	t_variable;

typedef struct s_var_list
{
	t_variable	**list;
	int list_size;
	int list_len;

}	t_var_list;

typedef struct s_bucket
{
	struct s_bucket *next;
	char			*key;
	void			*data;
	unsigned int	khash;
	int				times_found;

}	t_bucket;

typedef struct s_hash
{
	t_bucket	**bucket_array;
	int			nbuckets;
	int			nentries;

}	t_hash;

typedef struct s_context
{
	char				*name;
	int					scope;
	int					flags;
	struct s_context	*up;
	struct s_context	*down;
	t_hash				*table;

}	t_context;

typedef struct s_minishell
{
	t_redirect	*redir_stack[HEREDOC_MAX];
	int			need_here_doc;
	t_context	*global_variables;
	int			last_command_exit_value;

}				t_minishell;

char			*decode_prompt(char *ps1);

#endif
