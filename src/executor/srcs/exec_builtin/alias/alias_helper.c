/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:46:32 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:26:36 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alias.h"

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

void	list_alias(t_minishell *minishell)
{
	char	**alias;
	int		i;
	char	*key;
	char	*joined_str;

	alias = make_var_export_array(minishell->alias_variables, 1);
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

int	ft_alias_with_value(char *arg, t_minishell *minishell)
{
	char	*key;

	key = ft_get_key(arg);
	if (!key)
	{
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: alias: `", arg),
				"': invalid alias name"), 2);
		return (1);
	}
	unbind_variable(ft_get_key(arg), minishell->alias_variables);
	if (!bind_variable(ft_get_key(arg), ft_strchr(arg, '=') + 1,
			minishell->alias_variables))
		return (1);
	return (0);
}

int	ft_alias_without_value(char *arg, t_minishell *minishell)
{
	t_variable	*v;
	char		*joined_str;

	v = find_variable(arg, minishell->alias_variables);
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
