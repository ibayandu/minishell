/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:12 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 19:14:11 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "token.h"

void	*init_lexer(char *input);
t_token	*get_next_token(void);
t_token	*get_current_token(void);
int		consume_token(t_token_type type);

#endif // LEXER_H
