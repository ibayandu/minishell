/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makers.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:26:22 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/12 20:02:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAKERS_H
# define MAKERS_H

# include "structs.h"

t_word		*alloc_word_desc(void);
t_word		*make_bare_word(const char *string);
t_word		*make_word_flags(t_word *w, const char flags);
t_word		*make_word(const char *string, const char flags);
t_word		*make_word_from_token(int token);

t_command	*make_command(t_cmd_type type, t_simple_cmd *pointer);
t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type);
t_command	*make_subshell_command(t_command *command);

t_word_list	*make_word_list(t_word *word, t_word_list *wlink);
t_command	*make_bare_simple_command(void);
t_command	*make_simple_command(t_element *element, t_command *command);
t_command	*clean_simple_command(t_command *command);

t_redirect	*make_redirection(t_word *source, t_redir_type redir_type,
				t_word *dest_and_filename);
void		make_here_document(t_redirect *temp);

#endif // MAKERS_H
