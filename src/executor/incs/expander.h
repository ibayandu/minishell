/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/03 12:36:43 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "libft.h"
# include "libhash.h"
# include "parser_structs.h"

# define DEFAULT_INITIAL_ARRAY_SIZE 112
# define VARIABLES_HASH_BUCKETS 1024

// Globbing Flags
# define GX_ALLDIRS 4
# define GX_NULLDIR 16
# define GX_ADDCURDIR 256

// Make Path Flags
# define MP_DOTILDE 1
# define MP_DOCWD 2
# define MP_RMDOT 4
# define MP_IGNDOT 8

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

typedef struct s_finddir_args
{
	char	*pat;
	char	*sdir;
	int		flags;
	t_list	**ep;
	int		*np;
}			t_finddir_args;

t_hash		*create_variable_tables(int table_index);
t_variable	*find_variable(const char *name, t_hash *ht);
t_variable	*bind_variable(const char *name, char *value, t_hash *ht);
int			unbind_variable(const char *name, t_hash *ht);

t_word_list	*list_append(t_word_list *head, t_word_list *tail);
char		**strvec_sort(char **input, int is_asc);
int			legal_identifier(char *name);
t_word_list	*word_list_split(t_word_list *list);

t_word_list	*expand_word_list(t_word_list *list, int *exit_code);
t_word_list	*expand_word(t_word *word, int quoted, int *expanded_something,
				int exit_code);
t_word		*param_expand(char *string, int *sindex, int *expanded_something,
				int exit_code);

char		*string_extract_double_quoted(char *string, int *sindex);
char		*string_list(t_word_list *list);

char		**make_var_export_array(t_hash *ht, int is_export);
t_word_list	*list_string(char *string, char *seperators);

char		**glob_vector(char *pat, char *dir, int flags);
t_word_list	*glob_list(t_word_list *tlist);

int			testdir(char *dir);
char		**arraydir(char *dir, char **array);
t_list		*finddir(t_finddir_args args);

char		*redir_expand(t_word *redir_word, int *exit_code);
char		*here_document_expand(t_word *document, int *exit_code);

#endif // EXPANDER_H
