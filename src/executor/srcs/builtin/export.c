/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:43:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 22:23:36 by yzeybek          ###   ########.tr       */
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

char	**sort_strings(char **input)
{
	size_t	count;
	size_t	i;
	char	*tmp;
	size_t	j;

	count = count_strings(input);
	if (!count)
		return (NULL);
	char **sorted = ft_malloc((count + 1) * sizeof(char *));
	if (!sorted)
		return NULL;
	i = -1;
	while (++i < count)
		sorted[i] = ft_strdup(input[i]);
	sorted[count] = NULL;

	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - 1 - i)
		{
			if (ft_strncmp(sorted[j], sorted[j + 1], ft_strlen(sorted[j + 1])) > 0)
			{
				tmp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = tmp;
			}
		}
	}

	return sorted;
}

static void	list_env(t_minishell *minishell)
{
	char	**env;
	int			i;

	i = 0;
	env = make_var_export_array(minishell->global_variables);
	env = sort_strings(env);
	while (env && env[i])
		ft_putendl_fd(ft_strjoin(ft_strjoin("declare -x \"", env[i++]), "\""), 1);
}

static char	*ft_get_key(char *arg)
{
	int		i;
	char	*ret;

	ret = ft_strdup(arg);
	i = ft_strchr(ret, '=') - ret;
	ret[i] = '\0';
	return (ret);
}

static int	ft_export_with_value(char *arg, t_minishell *minishell)
{
	unbind_variable(ft_get_key(arg), minishell);
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1, minishell))
	{
		perror("export");
		return (1);
	}
	return (0);
}

static int	ft_export_without_value(char *arg, t_minishell *minishell)
{
	char		*val;
	t_variable	*v;

	val = NULL;
	v = find_variable_internal(arg, minishell);
	if (v)
		val = v->value;
	if (!bind_variable(arg, val, minishell))
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
//todo key verilmediğinde hata vermiyor örnek olarak export =5 dediğinde çalışıyor yani saçma veya export = çalışıyor bu da saçma
