/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:34:23 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/05 09:58:24 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtin.h"
#include "expander.h"

static char	*ft_get_key(char *arg)
{
	int		i;
	char	*ret;

	ret = ft_strdup(arg);
	i = ft_strchr(ret, '=') - ret;
	ret[i] = '\0';
	if (ft_isdigit(ret[0]))
		return (NULL);
	i = 0;
	while (ft_isalnum(ret[i]) || ret[i] == '_' || ret[i] == '.')
		i++;
	if (i)
		return (ret);
	return (NULL);
}

static void	list_alias(void)
{
	char	**alias;
	int		i;
	char	*key;
	char	*joined_str;

	alias = make_var_export_array(create_tables(1), 1);
	alias = strvec_sort(alias, 1);
	i = -1;
	while (alias && alias[++i])
	{
		if (ft_strchr(alias[i], '='))
		{
			key = ft_get_key(alias[i]);
			joined_str = ft_strjoin(ft_strjoin("alias ", key), "=");
			joined_str = ft_strjoin(joined_str, "'");
			joined_str = ft_strjoin(joined_str, ft_strchr(alias[i], '=') + 1);
			joined_str = ft_strjoin(joined_str, "\'");
			ft_putendl_fd(joined_str, 1);
		}
	}
}

static int	ft_alias_with_value(char *arg)
{
	char	*key;

	key = ft_get_key(arg);
	if (!key)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: alias: `", arg),
				"': invalid alias name"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), create_tables(1));
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1,
			create_tables(1)))
		return (1);
	return (0);
}

static int	ft_alias_without_value(char *arg)
{
	t_variable	*v;
	char		*joined_str;

	v = find_variable(arg, create_tables(1));
	if (!v)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: alias: ", arg),
				": not found"), 2);
		return (1);
	}
	joined_str = ft_strjoin(ft_strjoin("alias ", v->name), "=");
	joined_str = ft_strjoin(joined_str, "'");
	joined_str = ft_strjoin(joined_str, v->value);
	ft_putendl_fd(ft_strjoin(joined_str, "\'"), 1);
	return (0);
}

int	builtin_alias(char **argv)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_alias();
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_alias_with_value(argv[i]))
				return (1);
		}
		else
		{
			if (ft_alias_without_value(argv[i]))
				return (1);
		}
		i++;
	}
	return (0);
}
