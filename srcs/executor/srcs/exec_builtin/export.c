/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:43:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/05 09:59:22 by yzeybek          ###   ########.tr       */
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
	while (ft_isalnum(ret[i]) || ret[i] == '_')
		i++;
	if (i)
		return (ret);
	return (NULL);
}

static void	list_env(void)
{
	char	**env;
	int		i;
	char	*key;
	char	*joined_str;

	env = make_var_export_array(create_tables(0), 1);
	env = strvec_sort(env, 1);
	i = -1;
	while (env && env[++i])
	{
		if (ft_strchr(env[i], '='))
		{
			key = ft_get_key(env[i]);
			joined_str = ft_strjoin(ft_strjoin(ft_strjoin("declare -x ", key),
						"="), "\"");
			joined_str = ft_strjoin(joined_str, ft_strchr(env[i], '=') + 1);
			ft_putendl_fd(ft_strjoin(joined_str, "\""), 1);
		}
		else
			ft_putendl_fd(ft_strjoin("declare -x ", env[i]), 1);
	}
}

static int	ft_export_with_value(char *arg)
{
	char	*key;

	key = ft_get_key(arg);
	if (!key)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg),
				"': not a valid identifier"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), create_tables(0));
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1,
			create_tables(0)))
		return (1);
	return (0);
}

static int	ft_export_without_value(char *arg)
{
	int	i;

	i = 0;
	if (arg && *arg && !ft_isdigit(arg[i]))
	{
		while (ft_isalnum(arg[i]) || arg[i] == '_')
			i++;
	}
	if (!i)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg),
				"': not a valid identifier"), 2);
		return (1);
	}
	if (!bind_variable(arg, NULL, create_tables(0)))
		return (1);
	return (0);
}

int	builtin_export(char **argv)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_env();
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_export_with_value(argv[i]))
				return (1);
		}
		else
		{
			if (ft_export_without_value(argv[i]))
				return (1);
		}
		i++;
	}
	return (0);
}
