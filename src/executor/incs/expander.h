/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:06:33 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 15:08:11 by yzeybek          ###   ########.tr       */
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

t_word_list	*expand_word_list(t_word_list *list, int is_redir);

#endif /* EXPANDER_H*/
