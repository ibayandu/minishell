/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/14 01:50:39 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "libft.h"
# include "libhash.h"
# include "parser.h"

# define DEFAULT_INITIAL_ARRAY_SIZE 112
# define VARIABLES_HASH_BUCKETS 1024
# define CTLESC '\001'
# define DCTLESC "\001\001"

// Globbing Flags
# define GX_ALLDIRS 4
# define GX_NULLDIR 16
# define GX_ADDCURDIR 256

// Make Path Flags
# define MP_DOTILDE 1
# define MP_DOCWD 2
# define MP_RMDOT 4
# define MP_IGNDOT 8

typedef struct s_glob_state
{
	int		flags;
	char	*filename;
	char	*dirname;
	int		dirlen;
	int		is_all_dctlesc;

}	t_glob_state;

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

char		**glob_dirs(t_glob_state *glob_state, char *dirname, int dirlen,
				int flags);
char		**glob_filename(char *pathname, int flags);
char		**glob_filename_empty(char *directory_name, char **result);
char		**glob_filename_only(char *directory_name, int directory_len,
				char *filename, int flags);
char		**glob_vector(char *pat, char *dir, int flags);
t_word_list	*glob_list(t_word_list *tlist);

t_word_list	*expand_word_list(t_word_list *list, int *exit_code);

char		*redir_expand(t_word *redir_word, int *exit_code);
char		*here_document_expand(t_word *document, int *exit_code);

t_word		*param_expand(char *str, int *sindex, int *is_expand,
				int exit_code);

t_word_list	*expand_word(t_word *word, int quoted, int *expand, int exit_code);

int			testdir(char *dir);
char		**arraydir(char *dir, char **array);
void		finddir(char **r, int *count, t_list **lastlink);

t_word_list	*glob_restar(t_word_list *list);
int			glob_pattern(char *string);
int			glob_match(const char *pattern, const char *string);
char		*glob_makepath(char *path, char *dir, int flags);

int			strvec_len(char **array);
char		**strvec_cpy(char **dest, char **src);
char		**strvec_shift(char **vec);
char		**strvec_realloc(char **old, size_t new_items);

t_word_list	*list_append(t_word_list *head, t_word_list *tail);
char		**make_var_export_array(t_hash *ht, int is_export);

char		**strvec_sort(char **input, int is_asc);

t_word_list	*word_list_split(t_word_list *list);
t_word_list	*list_string(char *string, char *seperators);

char		*string_list(t_word_list *list);
char		*string_extract_double_quoted(char *string, int *sindex);

t_hash		*create_tables(int table_index);
t_variable	*find_variable(const char *name, t_hash *ht);
t_variable	*bind_variable(const char *name, char *value, t_hash *ht);
int			unbind_variable(const char *name, t_hash *ht);

t_variable	**map_over(int (*function)(t_variable *), t_hash *ht);

#endif // EXPANDER_H
