/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:50:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 19:04:59 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_H
# define PARSER_UTILS_H

# include <sys/types.h>
# include "parser_structs.h"
# include "lexer.h"

# define HEREDOC_MAX 16

t_word_list		*ft_revword(t_word_list *list);
t_redirect		*ft_revredir(t_redirect *list);
char			*string_quote_removal(char *string);
void			ft_panic(t_token *token, int *exit_code);
t_redir_type	ft_get_redir(t_token *token);
char			*ft_readline(char *prompt);
t_cnt_type		ft_get_cnt(t_token *token);
char			*string_extract_single_quoted(char *string, int *sindex);
t_redirect		**push_heredoc(t_redirect *r);
pid_t			gather_here_documents_fd(char ***paths,
					t_redirect **redir_stack);
int				read_heredoc_inputs(char **paths, t_redirect **redir_stack);

#endif // PARSER_UTILS_H
