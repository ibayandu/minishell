/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:50:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 00:51:18 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include "structs.h"

size_t			ft_mbrlen(const char *s, size_t max_len);
int				ft_charflag(char c, int flag);
t_word_list		*ft_revword(t_word_list *list);
t_redirect		*ft_revredir(t_redirect *list);
char			*string_quote_removal(char *string);
void			ft_panic(void);
t_redir_type	ft_get_redir(void);
t_cnt_type		ft_get_cnt(void);
char			*ft_readline(char *prompt);

#endif // UTILS_H
