/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:59:39 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 22:39:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "expander.h"
#include "minishell.h"
#include "get_next_line.h"
#include "libft.h"
#include "lexer.h"
#include "parsers.h"
#include "executor.h"
#include "utils.h"

void	execute_begin_file(char *filepath, t_minishell *minishell)
{
	int			fd;
	char		*content;
	t_command	*cmd;

	if (!filepath)
		return ;
	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return ;
	content = get_all_line(fd);
	close(fd);
	if (content)
	{
		if (init_lexer(ft_strjoin(content, "\n")))
		{
			cmd = parse_inputunit(minishell);
			if (!ft_heredoc(minishell) && cmd)
				minishell->last_command_exit_value = execute_command(cmd,
						minishell);
		}
	}
}

void	execute_minirc(t_minishell *minishell)
{
	t_variable	*v;

	v = find_variable("HOME", minishell->global_variables);
	if (v)
		execute_begin_file(ft_strjoin(v->value, "/.minirc"), minishell);
}

int	same_file(const char *path1, const char *path2, struct stat *stp1, struct stat *stp2)
{
	struct stat	st1;
	struct stat	st2;

	if (!stp1)
	{
		if (stat(path1, &st1) != 0)
			return (0);
		stp1 = &st1;
	}
	if (!stp2)
	{
		if (stat(path2, &st2) != 0)
			return (0);
		stp2 = &st2;
	}
	return ((stp1->st_dev == stp2->st_dev) && (stp1->st_ino == stp2->st_ino));
}

char	*sh_canonpath(char *path)
{
	struct stat	sb;
	char		stub_char;
	char		*result;
	char		*p;
	char		*q;
	char		*base;
	char		*dotdot;
	int			rooted;
	int			double_slash_path;
	char		c;

	result = ft_strdup(path);
	rooted = path[0] == '/';
	if (rooted)
	{
		stub_char = '/';
		base = result + 1;
		double_slash_path = path[0] == '/' && path[1] == '/' && path[2] != '/';
		base += double_slash_path;
	}
	else
	{
		stub_char = '.';
		base = result;
		double_slash_path = 0;
	}

	dotdot = base;
	q = dotdot;
	p = q;
	while (*p)
	{
		if (p[0] == '/')
			p++;
		else if(p[0] == '.' && (p[1] == '/' || p[1] == 0))
			p += 1;
		else if (p[0] == '.' && p[1] == '.' && (p[2] == '/' || p[2] == 0))
		{
			p += 2;
			if (q > dotdot)
			{
				c = *q;
				*q = '\0';
				if(!(stat(result, &sb) == 0 && S_ISDIR(sb.st_mode)))
					return (NULL);
				*q = c;
				while (--q > dotdot && *q == '/')
					;
			}
			else if (rooted == 0)
			{
				if (q != base)
				*q++ = '/';
				*q++ = '.';
				*q++ = '.';
				dotdot = q;
			}
		}
		else
		{
			if (q != base)
				*q++ = '/';
			while (*p && (*p != '/'))
				*q++ = *p++;
			c = *q;
			*q = '\0';
			if (!(stat(result, &sb) == 0 && S_ISDIR(sb.st_mode)))
				return ((char *)NULL);
			*q = c;
		}
	}
	if (q == result)
		*q++ = stub_char;
	*q = '\0';
	if (path[0] == '/' && path[1] == '/' && path[2] != '/' && double_slash_path == 0)
	{
		if (result[2] == '\0')
			result[1] = '\0';
		else
			ft_memmove(result, result + 1, ft_strlen(result + 1) + 1);
	}
	return (result);
}

void	set_pwd(t_minishell *minishell)
{
	t_variable	*temp_var;
	t_variable	*home_var;
	char		*temp_string;
	char		*home_string;
	char		*current_dir;

	home_var = find_variable("HOME", minishell->global_variables);
	home_string = home_var ? home_var->value : NULL;
	temp_var = find_variable("PWD", minishell->global_variables);
	if (temp_var)
		temp_string = temp_var->value;
	if (temp_var && temp_var->value && temp_string[0] == '/' && same_file(temp_string, ".", NULL, NULL))
	{
		current_dir = sh_canonpath(temp_string);
		if (!current_dir)
			current_dir = ft_strdup(getcwd(0,0));
		else
			temp_var = bind_variable("PWD", current_dir, minishell->global_variables);
	}
	else if (home_string && same_file (home_string, ".", NULL, NULL))
		temp_var = bind_variable ("PWD", home_string, minishell->global_variables);
	else
	{
		temp_string = ft_strdup(getcwd(0,0));
		if (temp_string)
			temp_var = bind_variable ("PWD", temp_string, minishell->global_variables);
	}
	temp_var = find_variable("OLDPWD", minishell->global_variables);
	if (!temp_var || !temp_var->value)
		temp_var = bind_variable ("OLDPWD", NULL, minishell->global_variables);
}

void	adjust_shell_level(int change)
{
	char		*new_level;
	long		old_level;
	int			shell_level;
	t_variable	*temp_var;

	old_level = 0;
	temp_var = find_variable("SHLVL", create_variable_tables(0));
	if (temp_var)
		old_level =  ft_atoi(temp_var->value);
	shell_level = old_level + change;
	new_level = ft_itoa(shell_level);
	unbind_variable("SHLVL", create_variable_tables(0));
	bind_variable("SHLVL", new_level, create_variable_tables(0));
}

void	bind_environments(char **env)
{
	char	*key;
	char	*value;
	int		i;
	int		j;

	j = -1;
	while (env && env[++j])
	{
		key = env[j];
		value = env[j];
		i = 0;
		while (*value && *value != '=')
			value++;
		if (value[-1] == '=')
			i = value - key - 1;
		if (!i)
			continue ;
		key[i] = '\0';
		i = 0;
		while (!ft_isdigit(key[0]) && (ft_isalnum(key[i]) || key[i] == '_' || key[i] == '.'))
			i++;
		if (i)
			bind_variable(key, value, create_variable_tables(0));
		key[i] = '=';
	}
}

void	initialize_shell_variables(char **env, int *exit_code)
{
	*exit_code = 0;
	create_variable_tables(0);
	bind_environments(env);
	set_pwd(exit_code);
	execute_minirc(exit_code);
	bind_variable("PATH", DEFAULT_PATH_VALUE, create_variable_tables(0));
	bind_variable("HOME", ft_strdup("/"), create_variable_tables(0));
	adjust_shell_level(1);
}
