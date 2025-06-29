/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 20:20:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/28 21:57:27 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>

char	*get_next_line(int fd);
char	*find_newline(char *str);
char	*gnl_strjoin(char *s1, char *s2);
size_t	gnl_strlen(char *str);
size_t	gnl_strlcpy(char *dst, char *src, size_t dstsize);
void	*del(void *data);

#endif
