/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:43:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:27:11 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

static char	*ft_get_key(char *arg)
{
	int		i;
	char	*ret;

	ret = ft_strdup(arg);
	i = ft_strchr(ret, '=') - ret;
	ret[i] = '\0';
	if (*ret && legal_identifier(ret))
		return (ret);
	return (NULL);
}

static void	list_env(t_minishell *minishell)
{
	char	**env;
	int		i;
	char	*key;
	char	*joined_str;

	env = make_var_export_array(minishell->global_variables, 1);
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

static int	ft_export_with_value(char *arg, t_minishell *minishell)
{
	char	*key;

	key = ft_get_key(arg);
	if (!key)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg),
				"': not a valid identifier"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), minishell->global_variables);
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1,
			minishell->global_variables))
		return (1);
	return (0);
}

static int	ft_export_without_value(char *arg, t_minishell *minishell)
{
	if (!legal_identifier(arg))
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg),
				"': not a valid identifier"), 2);
		return (1);
	}
	if (!bind_variable(arg, NULL, minishell->global_variables))
		return (1);
	return (0);
}

int	builtin_export(char **argv, t_minishell *minishell)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_env(minishell);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_export_with_value(argv[i], minishell) != 0)
				return (1);
		}
		else
		{
			if (ft_export_without_value(argv[i], minishell) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
