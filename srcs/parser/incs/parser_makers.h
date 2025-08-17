/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_makers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:26:22 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/11 04:23:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_MAKERS_H
# define PARSER_MAKERS_H

# include "parser_structs.h"

t_word		*alloc_word_desc(void);
t_word		*make_bare_word(const char *string);
t_word		*make_word(const char *string, const char flags);

t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type);
t_command	*make_subshell_command(t_command *command);

t_word_list	*make_word_list(t_word *word, t_word_list *wlink);
t_command	*make_bare_simple_command(void);
t_command	*make_simple_command(t_element *element, t_command *command);
t_command	*clean_simple_command(t_command *command);

t_redirect	*make_redirection(t_word *source, t_redir_type redir_type,
				t_word *dest_and_filename, int flags);
void		make_here_document_fd(t_redirect *temp, int fd);

#endif // PARSER_MAKERS_H
