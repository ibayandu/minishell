/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:59:39 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/05 21:16:21 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "expander.h"
#include "minishell.h"
#include "libgnl.h"
#include "libft.h"
#include "lexer.h"
#include "parsers.h"
#include "executor.h"

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
			if (minishell->need_here_doc)
				gather_here_documents(minishell);
			if (cmd)
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

void	adjust_shell_level(int change, t_minishell *minishell)
{
	char		new_level[5];
	char		*old_SHLVL;
	long		old_level;
	int			shell_level;
	t_variable	*temp_var;

	temp_var = find_variable("SHLVL", minishell->global_variables);
	old_SHLVL =  temp_var ? temp_var->value : NULL;
	old_level = ft_atoi(old_SHLVL);
	shell_level = old_level + change;
	if (shell_level < 0)
		shell_level = 0;
	else if (shell_level >= 1000)
	{
		//internal_warning (_("shell level (%d) too high, resetting to 1"), shell_level);
		shell_level = 1;
	}
	if (shell_level < 10)
	{
		new_level[0] = shell_level + '0';
		new_level[1] = '\0';
	}
	else if (shell_level < 100)
	{
		new_level[0] = (shell_level / 10) + '0';
		new_level[1] = (shell_level % 10) + '0';
		new_level[2] = '\0';
	}
	else if (shell_level < 1000)
	{
		new_level[0] = (shell_level / 100) + '0';
		old_level = shell_level % 100;
		new_level[1] = (old_level / 10) + '0';
		new_level[2] = (old_level % 10) + '0';
		new_level[3] = '\0';
	}
	unbind_variable("SHLVL", minishell->global_variables);
	temp_var = bind_variable("SHLVL", new_level, minishell->global_variables);
}

void	initialize_shell_variables(char **env, t_minishell *minishell)
{
	char		*name;
	char		*string;
	int			c;
	int			char_index;
	int			string_index;

	minishell->last_command_exit_value = 0;
	create_variable_tables(minishell);
	string_index = 0;
	while (env && (string = env[string_index++]))
	{
		char_index = 0;
		name = string;
		while ((c = *string++) && c != '=')
			;
		if (string[-1] == '=')
			char_index = string - name - 1;
		if (!char_index)
			continue;
		name[char_index] = '\0';
		if (legal_identifier(name))
			bind_variable(name, string, minishell->global_variables);
		name[char_index] = '=';
	}
	set_pwd(minishell);
	execute_minirc(minishell);
	bind_variable("PATH", DEFAULT_PATH_VALUE, minishell->global_variables);
	bind_variable("TERM", "dumb", minishell->global_variables);
	adjust_shell_level(1, minishell);
	bind_variable("HOME", ft_strdup("/"), minishell->global_variables);
	bind_variable("SHELL", "/bin/sh", minishell->global_variables);
}
