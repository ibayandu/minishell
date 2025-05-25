/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:48:41 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 11:55:13 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERS_H
# define PARSERS_H

#include "structs.h"

t_command	*parse_inputunit();
t_redirect	*parse_redirection();
t_element	*parse_simple_command_element();
t_redirect	*parse_redirection_list();
t_command	*parse_simple_command();
t_command	*parse_command();
t_command	*parse_list();
t_command	*parse_simple_list();
t_command	*parse_pipeline();
t_command	*parse_compound_list(void);
int 		parse_list_terminator(void);
void		parse_newline_list(void);

#endif // PARSERS_H
