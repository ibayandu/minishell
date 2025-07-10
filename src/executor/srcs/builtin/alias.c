/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:34:23 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/05 22:42:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

static int	alias_identifier(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (i);
	if (ft_isdigit(name[i]))
		return (i);
	while (ft_isalnum(name[i]) || name[i] == '_' || name[i] == '.')
		i++;
	return (!name[i]);
}

static char	*ft_get_key(char *arg)
{
	int		i;
	char	*ret;

	ret = ft_strdup(arg);
	i = ft_strchr(ret, '=') - ret;
	ret[i] = '\0';
	if (*ret && alias_identifier(ret))
		return (ret);
	return (NULL);
}

static void	list_alias(t_minishell *minishell)
{
	char	**alias;
	int		i;
	char	*key;

	alias = make_var_export_array(minishell->alias_variables, 1);
	alias = strvec_sort(alias, 1);
	i = -1;
	while (alias && alias[++i])
	{
		if (ft_strchr(alias[i], '='))
		{
			key = ft_get_key(alias[i]);
			ft_putendl_fd(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin("alias ", key), "="), "'"), ft_strchr(alias[i], '=') + 1), "\'"), 1);
		}
	}
}

static int	ft_alias_with_value(char *arg, t_minishell *minishell)
{
	char	*key;

	key = ft_get_key(arg);
	if (!key)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: alias: `", arg), "': invalid alias name"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), minishell->alias_variables);
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1, minishell->alias_variables))
		return (1);
	return (0);
}

static int	ft_alias_without_value(char *arg, t_minishell *minishell)
{
	t_variable	*v;

	v = find_variable(arg, minishell->alias_variables);
	if (!v)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: alias: ", arg), ": not found"), 2);
		return (1);
	}
	ft_putendl_fd(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin("alias ", v->name), "="), "'"), v->value), "\'"), 1);
	return (0);
}

int	builtin_alias(char **argv, t_minishell *minishell)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_alias(minishell);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_alias_with_value(argv[i], minishell) != 0)
				return (1);
		}
		else
		{
			if (ft_alias_without_value(argv[i], minishell) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
