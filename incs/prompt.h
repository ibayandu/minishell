/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:24:41 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 04:33:02 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include <stddef.h>

char	*get_username(void);
char	*get_hostname_full(void);
int		get_hostname(char *buffer, size_t size);
char	*get_basename(const char *path);
char	*get_short_pwd(void);
size_t	handle_escape_sequence(const char *input, size_t i, char *result,
			size_t *j);
char	*replace_escape_sequences(const char *input);
void	remove_escape_blocks(const char *ps1, char *dst);
char	*get_prompt_sub(char c);
char	*get_prompt(void);

#endif // PROMPT_H
