/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:48:41 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/14 02:29:38 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERS_H
# define PARSERS_H

# include "structs.h"

t_command	*parse_inputunit(void);
t_redirect	*parse_redirection(void);
t_element	*parse_simple_command_element(void);
t_redirect	*parse_redirection_list(void);
t_command	*parse_simple_command(void);
t_command	*parse_command(void);
t_command	*parse_list(void);
t_command	*parse_simple_list(void);
t_command	*parse_pipeline(void);
t_command	*parse_compound_list(void);
void		parse_newline_list(void);

#endif // PARSERS_H
