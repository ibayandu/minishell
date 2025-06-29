/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:47:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/28 21:57:13 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

static void	trim_line(char **content)
{
	char	*start;
	char	*temp;

	start = find_newline(*content);
	if (!start)
	{
		del(content);
		return ;
	}
	temp = malloc(gnl_strlen(start) * sizeof(char));
	if (!temp)
	{
		del(content);
		return ;
	}
	gnl_strlcpy(temp,
		(*content) + gnl_strlen(*content) - gnl_strlen(start) + 1,
		gnl_strlen(start));
	free(*content);
	*content = temp;
	if (!**content)
		del(content);
}

static void	get_line(char **content, char	**next_line)
{
	char	*start;
	size_t	length;

	start = find_newline(*content);
	length = gnl_strlen(*content) - gnl_strlen(start) + 2;
	*next_line = malloc(length * sizeof(char));
	if (!*next_line)
		return ;
	gnl_strlcpy(*next_line, *content, length);
}

static int	read_fd(int fd, char **content, char *buffer)
{
	char	*temp;
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read == -1)
	{
		del(content);
		return (-1);
	}
	if (!bytes_read)
		return (0);
	buffer[bytes_read] = '\0';
	temp = gnl_strjoin(*content, buffer);
	free(*content);
	*content = temp;
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*content;
	char		*next_line;
	char		*buffer;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!find_newline(content) && bytes_read > 0)
		bytes_read = read_fd(fd, &content, buffer);
	free(buffer);
	if (bytes_read == -1 || !content)
		return (NULL);
	get_line(&content, &next_line);
	trim_line(&content);
	return (next_line);
}
