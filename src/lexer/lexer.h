/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:12 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/15 17:37:45 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "token.h"

void	*init_lexer(char *input);
t_token	*get_next_token(void);
t_token	*get_current_token(void);
int		consume_token(t_token_type type);

#endif // LEXER_H
