/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:48:19 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 04:39:51 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "libmem.h"
#include "libgnl.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "expander.h"
#include "minishell.h"
#include "init.h"

static void	execute_minirc(int *exit_code)
{
	t_variable	*v;
	int			fd;
	char		*content;
	t_command	*cmd;

	v = find_variable("HOME", create_tables(0));
	if (!v)
		return ;
	fd = open(ft_strjoin(v->value, "/.minirc"), O_RDONLY);
	if (fd < 0)
		return ;
	content = gnl_all(fd);
	close(fd);
	if (content)
	{
		if (init_lexer(ft_strjoin(content, "\n")))
		{
			cmd = parse_inputunit(exit_code);
			if (!ft_heredoc(exit_code) && cmd)
				*exit_code = execute_command(cmd, exit_code);
		}
	}
}

static void	adjust_shell_level(int change)
{
	char		*new_level;
	long		old_level;
	int			shell_level;
	t_variable	*temp_var;

	old_level = 0;
	temp_var = find_variable("SHLVL", create_tables(0));
	if (temp_var)
		old_level = ft_atoi(temp_var->value);
	shell_level = old_level + change;
	new_level = ft_itoa(shell_level);
	unbind_variable("SHLVL", create_tables(0));
	bind_variable("SHLVL", new_level, create_tables(0));
}

static void	bind_environments(char **env)
{
	char	*key;
	char	*value;
	size_t	i;
	int		j;

	j = -1;
	while (env && env[++j])
	{
		key = env[j];
		value = env[j];
		i = 0;
		while (key[i] != '=' && !ft_isdigit(key[0]) && (ft_isalnum(key[i])
				|| key[i] == '_' || key[i] == '.'))
			i++;
		if (key[i] == '=')
			bind_variable(ft_strndup(key, i), value + i + 1, create_tables(0));
	}
}

static int	same_file(const char *path1, const char *path2)
{
	struct stat	st1;
	struct stat	st2;
	struct stat	*stp1;
	struct stat	*stp2;

	if (!path1 || !path2)
		return (0);
	if (stat(path1, &st1))
		return (0);
	stp1 = &st1;
	if (stat(path2, &st2))
		return (0);
	stp2 = &st2;
	return ((stp1->st_dev == stp2->st_dev) && (stp1->st_ino == stp2->st_ino));
}

void	initialize_shell_variables(char **env, int *exit_code)
{
	t_variable	*v;
	char		*tmp;

	*exit_code = 0;
	tmp = NULL;
	bind_environments(env);
	bind_variable("PATH", DEFAULT_PATH_VALUE, create_tables(0));
	bind_variable("TERM", "dumb", create_tables(0));
	bind_variable("SHELL", "/bin/sh", create_tables(0));
	adjust_shell_level(1);
	v = find_variable("PWD", create_tables(0));
	if (v)
		tmp = v->value;
	if (!(tmp && tmp[0] == '/' && same_file(tmp, ".")))
	{
		v = find_variable("HOME", create_tables(0));
		if (v)
			tmp = v->value;
		if (tmp && same_file(tmp, "."))
			bind_variable("PWD", tmp, create_tables(0));
		else
			bind_variable("PWD", ft_strdup(mem_absorb(getcwd(NULL, 0))),
				create_tables(0));
	}
	execute_minirc(exit_code);
}
