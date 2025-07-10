/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:50:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 15:39:33 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include "token.h"
# include "structs.h"
# include "minishell.h"

size_t			ft_mbrlen(const char *s, size_t max_len);
int				ft_charflag(char c, int flag);
t_word_list		*ft_revword(t_word_list *list);
t_redirect		*ft_revredir(t_redirect *list);
char			*string_quote_removal(char *string);
void			ft_panic(t_token *token, t_minishell *minishell);
t_redir_type	ft_get_redir(t_token *token);
char			*ft_readline(char *prompt);
t_cnt_type		ft_get_cnt(t_token *token);
size_t			add_mbqchar_body(char **dst, const char *src, size_t *si, size_t srcsize);
char			*string_extract_single_quoted(char *string, int *sindex);
void			push_heredoc(t_redirect *r, t_minishell *minishell);
int				ft_heredoc(t_minishell *minishell);

#endif // UTILS_H
