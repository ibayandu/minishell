/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:48:41 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/19 23:03:24 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERS_H
# define PARSERS_H

# include "structs.h"
# include "minishell.h"

t_command	*parse_inputunit(t_minishell *minishell);
t_redirect	*parse_redirection(t_minishell *minishell);
t_element	*parse_simple_command_element(t_minishell *minishell);
t_redirect	*parse_redirection_list(t_minishell *minishell);
t_command	*parse_simple_command(t_minishell *minishell);
t_command	*parse_command(t_minishell *minishell);
t_command	*parse_list(t_minishell *minishell);
t_command	*parse_simple_list(t_minishell *minishell);
t_command	*parse_pipeline(t_minishell *minishell);
t_command	*parse_compound_list(t_minishell *minishell);
void		parse_newline_list(void);
void		gather_here_documents(t_minishell *minishell);

#endif // PARSERS_H
