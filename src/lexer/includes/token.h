/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:59 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 14:28:42 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef enum
{
	T_WORD,
	T_ASSIGNMENT_WORD,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_SQUOTE,
	T_DQUOTE,
	T_PIPE,
	T_AND_IF,
	T_OR_IF,
	T_LPARANTHESE,
	T_RPARANTHESE,
	T_EOF
}					t_token_type;

typedef struct
{
	t_token_type	token_type;
	char			*value;
}					t_token;

#endif // TOKEN_H
