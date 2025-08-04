/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:48:41 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 18:13:46 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_FUNCS_H
# define PARSER_FUNCS_H

# include "parser_structs.h"

t_element	*parse_simple_command_element(int *exit_code);
t_command	*parse_simple_command(int *exit_code);
t_command	*parse_command(int *exit_code);
t_command	*parse_pipeline(int *exit_code);

t_redirect	*parse_redirection(int *exit_code);
t_redirect	*parse_redirection_list(int *exit_code);

t_command	*parse_list(int *exit_code);
t_command	*parse_simple_list(int *exit_code);
t_command	*parse_compound_list(int *exit_code);
void		parse_newline_list(void);

#endif // PARSER_FUNCS_H
