/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/28 09:27:11 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "structs.h"
# include "libft.h"
# include "minishell.h"

# define DEFAULT_INITIAL_ARRAY_SIZE 112
# define VARIABLES_HASH_BUCKETS 1024
# define HASH_REHASH_MULTIPLIER 4
# define DEFAULT_HASH_BUCKETS 128
# define FNV_OFFSET 2166136261

// Globbing Flags
# define GX_ALLDIRS 4
# define GX_NULLDIR 16
# define GX_ADDCURDIR 256

// Make Path Flags
# define MP_DOTILDE 1
# define MP_DOCWD 2
# define MP_RMDOT 4
# define MP_IGNDOT 8

t_word_list *list_append(t_word_list *head, t_word_list *tail);
char		**strvec_sort(char **input, int is_asc);
int			legal_identifier(char *name);
void		create_variable_tables(t_minishell *minishell);
t_word_list	*expand_word_list(t_word_list *list, t_minishell *minishell);
t_word_list	*word_list_split(t_word_list *list);
t_word_list	*expand_word(t_word *word, int quoted, int *expanded_something, t_minishell *minishell);
char		*string_extract_double_quoted(char *string, int *sindex);
char		*string_list(t_word_list *list);
t_variable	*find_variable(const char *name, t_minishell *minishell);
t_word		*param_expand(char *string, int *sindex, int *expanded_something, t_minishell *minishell);
t_variable	*bind_variable(const char *name, char *value, t_minishell *minishell);
int			unbind_variable(const char *name, t_minishell *minishell);
char		**make_var_export_array(t_hash *ht, int is_export);
t_word_list	*list_string(char *string, char *seperators);

char		**glob_vector(char *pat, char *dir, int flags);
t_word_list	*glob_list(t_word_list *tlist);

int			testdir(char *dir);
char		**arraydir(char *dir, char **array);
t_list		*finddir(char *pat, char *sdir, int flags, t_list **ep, int *np);

char		*redir_expand(t_word *redir_word, t_minishell *minishell);
char		*here_document_expand(t_word *document, t_minishell *minishell);

#endif /* EXPANDER_H*/
