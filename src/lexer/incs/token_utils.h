/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:40:27 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 22:08:44 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

# include "libft.h"
# include "token.h"

t_token	*create_token(t_token_type type, char *value);
char	*get_token_str(char *input);

#endif // TOKEN_UTILS_H
