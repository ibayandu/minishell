/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:43:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 06:50:35 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

size_t	count_strings(char **arr)
{
	size_t	count;

	count = 0;
	while (arr && arr[count])
		count++;
	return (count);
}

char	**strvec_sort(char **input, int is_asc)
{
	size_t count;

	count = count_strings(input);
	if (count == 0)
		return NULL;

	char **sorted = ft_malloc((count + 1) * sizeof(char *));
	if (!sorted)
		return NULL;
	for (size_t i = 0; i < count; i++)
		sorted[i] = ft_strdup(input[i]);
	sorted[count] = NULL;
	for (size_t i = 0; i + 1 < count; i++)
	{
		for (size_t j = 0; j + 1 < count - i; j++)
		{
			int cmp = ft_strcmp(sorted[j], sorted[j+1]);
			if ((is_asc && cmp > 0) || (!is_asc && cmp < 0))
			{
				char *tmp = sorted[j];
				sorted[j] = sorted[j+1];
				sorted[j+1] = tmp;
			}
		}
	}

	return sorted;
}

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

	env = make_var_export_array(minishell->global_variables, 1);
	env = strvec_sort(env, 1);
	i = -1;
	while (env && env[++i])
	{
		if (ft_strchr(env[i], '='))
		{
			key = ft_get_key(env[i]);
			ft_putendl_fd(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin("declare -x ", key), "="), "\""), ft_strchr(env[i], '=') + 1), "\""), 1);
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
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg), "': not a valid identifier"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), minishell);
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1, minishell))
		return (1);
	return (0);
}

static int	ft_export_without_value(char *arg, t_minishell *minishell)
{
	if (!legal_identifier(arg))
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: export: `", arg), "': not a valid identifier"), 2);
		return (1);
	}
	if (!bind_variable(arg, NULL, minishell))
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
