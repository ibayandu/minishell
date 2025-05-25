/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:59 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/25 11:57:27 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

#include "libft.h"

# define F_DOLLAR 1
# define F_QUOTED 2
# define F_SINGLE_QUOTE 4
# define F_DOUBLE_QUOTE 8
# define F_ASSIGNMENT 16
# define F_STAR 32

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
	T_NUMBER_LESS,
	T_NUMBER_GREAT,
	T_NUMBER_DLESS,
	T_NUMBER_DGREAT,
	T_PIPE,
	T_LPARANTHESE,
	T_RPARANTHESE,
	T_NL,
	T_EOF
}					t_token_type;

/// @brief token listesini temsil eden bir yapı.
typedef struct s_token
{
	t_token_type	token_type;
	char			*value;
	char			flags;
}					t_token;

#endif // TOKEN_H
