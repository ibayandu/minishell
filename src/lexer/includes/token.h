/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:59 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 15:01:29 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../../lib/libft/libft.h"

typedef enum e_token_type
{
	T_WORD,
	T_ASSIGNMENT_WORD,
	T_NAME,
	T_NEWLINE,
	T_IO_NUMBER,
	T_AND_IF,
	T_OR_IF,
	T_DSEMI,
	T_DLESS,
	T_DGREAT,
	T_LESSAND,
	T_GREATAND,
	T_LESSGREAT,
	T_DLESSDASH,
	T_CLOBBER,
	T_If,
	T_Then,
	T_Else,
	T_Elif,
	T_Fi,
	T_Do,
	T_Done,
	T_Case,
	T_Esac,
	T_While,
	T_Until,
	T_For,
	T_Lbrace,
	T_Rbrace,
	T_Bang,
	T_In,
	T_EOF
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}					t_token;

typedef struct s_token_list
{
	t_list			*tokens;
	int				count;
}					t_token_list;

#endif // TOKEN_H
