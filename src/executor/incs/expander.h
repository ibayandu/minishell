/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 06:11:06 by yzeybek          ###   ########.tr       */
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

t_word_list		*expand_word_list(t_word_list *list, int is_redir, t_minishell *minishell);
t_word_list		*word_list_split(t_word_list *list);
t_word_list		*expand_word_internal(t_word *word, int quoted, int *expanded_something, t_minishell *minishell);
char			*string_extract_double_quoted(char *string, int *sindex);
char			*string_list_internal(t_word_list *list);
t_bucket		*hash_search(const char *string, t_hash *table);
unsigned int	hash_string(const char *s);
t_variable		*find_variable_internal(const char *name, t_minishell *minishell);
t_word			*param_expand(char *string, int *sindex, int *expanded_something, t_minishell *minishell);

#endif /* EXPANDER_H*/
