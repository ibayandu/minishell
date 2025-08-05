/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:47:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/05 16:53:29 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libmem.h"
#include "libgnl.h"
#include "gnl_utils.h"

static void	trim_line(char **content)
{
	char	*start;
	char	*temp;

	start = gnl_find_newline(*content);
	if (!start)
	{
		*content = NULL;
		return ;
	}
	temp = mem_malloc(gnl_strlen(start) * sizeof(char));
	gnl_strlcpy(temp, (*content) + gnl_strlen(*content) - gnl_strlen(start) + 1,
		gnl_strlen(start));
	*content = temp;
	if (!**content)
		*content = NULL;
}

static void	get_line(char **content, char **next_line)
{
	char	*start;
	size_t	length;

	start = gnl_find_newline(*content);
	length = gnl_strlen(*content) - gnl_strlen(start) + 2;
	*next_line = mem_malloc(length * sizeof(char));
	gnl_strlcpy(*next_line, *content, length);
}

static int	read_fd(int fd, char **content, char *buffer)
{
	char	*temp;
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read == -1)
	{
		*content = NULL;
		return (-1);
	}
	if (!bytes_read)
		return (0);
	buffer[bytes_read] = '\0';
	temp = gnl_strjoin(*content, buffer);
	*content = temp;
	return (1);
}

char	*gnl_one(int fd)
{
	static char	*content = NULL;
	char		*next_line;
	char		*buffer;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = mem_malloc((BUFFER_SIZE + 1) * sizeof(char));
	bytes_read = 1;
	while (!gnl_find_newline(content) && bytes_read > 0)
		bytes_read = read_fd(fd, &content, buffer);
	if (bytes_read == -1 || !content)
		return (NULL);
	get_line(&content, &next_line);
	trim_line(&content);
	return (next_line);
}

char	*gnl_all(int fd)
{
	char	*all;
	char	*line;
	char	*tmp;

	all = gnl_strdup("");
	line = gnl_one(fd);
	while (line)
	{
		tmp = gnl_strjoin(all, line);
		all = tmp;
		line = gnl_one(fd);
	}
	return (all);
}
