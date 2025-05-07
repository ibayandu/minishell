/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:59 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 03:20:33 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

/// @brief token türlerini temsil eden enum.
typedef enum s_token_type
{
	T_WORD,
	T_AND_IF,
	T_OR_IF,
	T_DLESS,
	T_DGREAT,
	T_GREAT,
	T_LESS,
	T_PIPE,
	T_LPARANTHESE,
	T_RPARANTHESE,
	T_EOF
}					t_token_type;

/// @brief token listesini temsil eden bir yapı.
typedef struct s_token
{
	t_token_type	token_type;
	char			*value;
}					t_token;

#endif // TOKEN_H
