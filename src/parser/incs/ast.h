/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 01:56:33 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 02:05:36 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/// @brief ast tipleri için enum.
typedef enum e_ast_type
{
	AST_COMMAD,
	AST_PIPE,
	AST_SEQUENCE,
	AST_SUBSHELL
}					t_ast_type;

/// @brief redirection işlemleri için enum.
typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}					t_redir_type;

/// @brief iç içe redirectionları tutabilen bir struct.
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

/// @brief tam işlevli bir ast struct.
typedef struct s_ast
{
	t_ast_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	char			**argv;
	t_redir			*redirs;

}					t_ast;

#endif // AST_H
