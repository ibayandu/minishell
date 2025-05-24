/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:50:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/22 22:36:39 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "structs.h"

size_t		ft_mbrlen(const char *s, size_t max_len);
int			ft_charflag(char c, int flag);
t_word_list	*ft_revword(t_word_list *list);
t_redirect	*ft_revredir(t_redirect *list);
char		*string_quote_removal(char *string, int quoted);

#endif // UTILS_H
