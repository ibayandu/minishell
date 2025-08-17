/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:04:15 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:21:00 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "parser_structs.h"
# include "parser_makers.h"
# include "parser_utils.h"

t_command	*parse_inputunit(int *exit_code);
int			ft_heredoc(int *exit_code);

#endif // PARSER_H
