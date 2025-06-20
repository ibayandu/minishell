/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 11:34:03 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include <stddef.h>
# include "structs.h"

# define DEFAULT_INITIAL_ARRAY_SIZE 112
# define UNQUOTED 0
# define PARTIALLY_QUOTED 1
# define WHOLLY_QUOTED 2

t_word_list	*list_append(t_word_list *head, t_word_list *tail);
t_word_list	*word_list_split(t_word_list *list);
t_word_list	*expand_word_list(t_word_list *list, int is_redir);
char	*string_extract_verbatim (char *string, size_t slen, int *sindex, char *charlist);

#endif /* EXPANDER_H*/
