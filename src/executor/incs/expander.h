/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/23 18:44:39 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "structs.h"
# include "minishell.h"

# define DEFAULT_INITIAL_ARRAY_SIZE 112
# define VARIABLES_HASH_BUCKETS 1024
# define HASH_REHASH_MULTIPLIER 4
# define DEFAULT_HASH_BUCKETS 128
# define FNV_OFFSET 2166136261

// Globbing Flags
# define GX_MARKDIRS 1
# define GX_MATCHDIRS 2
# define GX_ALLDIRS 4
# define GX_NULLDIR 16
# define GX_ADDCURDIR 256
# define GX_RECURSE 512
# define GX_SYMLINK 1024

// Make Path Flags
# define MP_DOTILDE 1
# define MP_DOCWD 2
# define MP_RMDOT 4
# define MP_IGNDOT 8

typedef struct s_glob
{
	struct s_glob	*next;
	char			*name;

}	t_glob;

t_word_list *list_append(t_word_list *head, t_word_list *tail);
char		**strvec_sort(char **input, int is_asc);
t_variable	*set_if_not(char *name, char *value, t_minishell *minishell);
int			legal_identifier(char *name);
void		create_variable_tables(t_minishell *minishell);
t_word_list	*expand_word_list(t_word_list *list, int is_redir, t_minishell *minishell);
t_word_list	*word_list_split(t_word_list *list);
t_word_list	*expand_word_internal(t_word *word, int quoted, int *expanded_something, t_minishell *minishell);
char		*string_extract_double_quoted(char *string, int *sindex);
char		*string_list_internal(t_word_list *list);
t_variable	*find_variable_internal(const char *name, t_minishell *minishell);
t_word		*param_expand(char *string, int *sindex, int *expanded_something, t_minishell *minishell);
t_variable	*bind_variable(const char *name, char *value, t_minishell *minishell);
char		**make_var_export_array(t_context *vcxt);
int			unbind_variable(const char *name, t_minishell *minishell);
t_word_list	*list_string(char *string, char *seperators);
t_word_list	*glob_expand_word_list(t_word_list *tlist, t_minishell *minishell);

#endif /* EXPANDER_H*/
