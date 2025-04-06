/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:53 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 01:12:52 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include "token.h"

void	handle_word_without_quotes(char *input, int *i, t_token *tokens,
			int *token_index);
void	handle_word_with_quotes(char *input, int *i, t_token *tokens,
			int *token_index);
#endif // LEXER_UTILS_H
