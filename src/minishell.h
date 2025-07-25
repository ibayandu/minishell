/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:14:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 15:45:40 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs.h"

# define DEFAULT_PATH_VALUE "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."
# define HEREDOC_MAX 16
# define MAX_BUFFER_SIZE 1024
# define PS1 "minishell> " // "\\u@\\H:\\w$ "
# define PS2 "> "
# define VOID (void)0

typedef struct s_variable
{
	char	*name;
	char	*value;
	int		is_export;

}	t_variable;

typedef struct s_var_list
{
	t_variable	**list;
	int			list_size;
	int			list_len;

}	t_var_list;

typedef struct s_bucket
{
	struct s_bucket *next;
	char			*key;
	void			*data;
	unsigned int	khash;

}	t_bucket;

typedef struct s_hash
{
	t_bucket	**bucket_array;
	int			nbuckets;
	int			nentries;

}	t_hash;

typedef struct s_minishell
{
	t_redirect			*redir_stack[HEREDOC_MAX];
	int					need_here_doc;
	t_hash				*global_variables;
	t_hash				*alias_variables;
	int					last_command_exit_value;

}	t_minishell;

char	*get_prompt(t_minishell *minishell);
void	setup_signals_exec();
void	discard_signals();
void	setup_signals_main();

#endif
