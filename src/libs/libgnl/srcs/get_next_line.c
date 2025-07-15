/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 19:47:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/15 17:46:56 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "gnl_utils.h"
#include <unistd.h>

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
	temp = ft_malloc(gnl_strlen(start) * sizeof(char));
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
	*next_line = ft_malloc(length * sizeof(char));
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

char	*get_next_line(int fd)
{
	static char	*content;
	char		*next_line;
	char		*buffer;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_malloc((BUFFER_SIZE + 1) * sizeof(char));
	bytes_read = 1;
	while (!gnl_find_newline(content) && bytes_read > 0)
		bytes_read = read_fd(fd, &content, buffer);
	if (bytes_read == -1 || !content)
		return (NULL);
	get_line(&content, &next_line);
	trim_line(&content);
	return (next_line);
}

char	*get_all_line(int fd)
{
	char	*all;
	char	*line;
	char	*tmp;

	all = gnl_strdup("");
	line = get_next_line(fd);
	while (line)
	{
		tmp = gnl_strjoin(all, line);
		all = tmp;
		line = get_next_line(fd);
	}
	return (all);
}
