/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:59:39 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 06:45:15 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <errno.h>
#include "expander.h"
#include "minishell.h"
#include "libft.h"

t_variable	*new_shell_variable(const char *name)
{
	t_variable	*entry;

	entry = ft_malloc(sizeof(t_variable));
	entry->name = ft_strdup(name);
	entry->value = NULL;
	entry->exportstr = NULL;
	entry->attributes = 0;
	entry->context = 0;
	return (entry);
}

t_context	*new_var_context(char *name, int flags)
{
  t_context	*vc;

  vc = ft_malloc(sizeof(t_context));
  vc->name = name ? ft_strdup(name) : (char *)NULL;
  vc->flags = flags;
  vc->up = vc->down = NULL;
  vc->table = NULL;
  return vc;
}

t_hash	*hash_create(int buckets)
{
	t_hash	*new_table;
	int		i;

	new_table = ft_malloc(sizeof(t_hash));
	if (!buckets)
		buckets = DEFAULT_HASH_BUCKETS;
	new_table->bucket_array = ft_malloc(buckets * sizeof(t_bucket *));
	new_table->nbuckets = buckets;
	new_table->nentries = 0;
	i = -1;
	while (++i < buckets)
		new_table->bucket_array[i] = NULL;
	return (new_table);
}

void	create_variable_tables(t_minishell *minishell)
{
	if (!minishell->global_variables)
	{
		minishell->global_variables = new_var_context(NULL, 0);
		minishell->global_variables->scope = 0;
		minishell->global_variables->table = hash_create(VARIABLES_HASH_BUCKETS);
	}
}

int	legal_identifier(char *name)
{
	if (!name)
		return (0);
	while (ft_isalnum(*name) || *name == '_')
		name++;
	return (!*name);
}

void	hash_rehash(t_hash *table, int nsize)
{
	int			osize;
	int			i;
	int			j;
	t_bucket	**old_bucket_array;
	t_bucket	*item;
	t_bucket	*next;

	if (!table || nsize == table->nbuckets)
		return ;
	osize = table->nbuckets;
	old_bucket_array = table->bucket_array;
	table->nbuckets = nsize;
	table->bucket_array = ft_malloc(table->nbuckets * sizeof(t_bucket *));
	i = -1;
	while (++i < table->nbuckets)
		table->bucket_array[i] = NULL;
	j = -1;
	while (++j < osize)
	{
		item = old_bucket_array[j];
		while (item)
		{
			next = item->next;
			i = item->khash & (table->nbuckets - 1);
			item->next = table->bucket_array[i];
			table->bucket_array[i] = item;
			item = next;
		}
	}
}

void	hash_grow(t_hash *table)
{
  int nsize;

	nsize = table->nbuckets * HASH_REHASH_MULTIPLIER;
	if (nsize > 0)
		hash_rehash(table, nsize);
}

t_bucket	*hash_insert(char *string, t_hash *table)
{
	t_bucket		*item;
	int				bucket;
	unsigned int	hv;

	if (!table)
		table = hash_create(0);
	item = NULL;
	if (table->nentries >= table->nbuckets * 2)
		hash_grow(table);
	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	item = ft_malloc(sizeof(t_bucket));
	item->next = table->bucket_array[bucket];
	table->bucket_array[bucket] = item;
	item->data = NULL;
	item->key = string;
	item->khash = hv;
	item->times_found = 0;
	table->nentries++;
	return (item);
}

t_variable	*make_new_variable(const char *name, t_hash *table, t_minishell *minishell)
{
	t_variable	*entry;
	t_bucket	*elt;

	entry = new_shell_variable(name);
	if (!minishell->global_variables)
		create_variable_tables(minishell);
	elt = hash_insert(ft_strdup(name), table);
	elt->data = (void *)entry;
	return (entry);
}

char	*make_variable_value(char *value)
{
	char	*retval;

	if (value)
	{
		if (*value)
			retval = ft_strdup(value);
		else
		{
			retval = ft_malloc(1);
			retval[0] = '\0';
		}
	}
	else
		retval = NULL;
	return retval;
}

t_variable	*bind_variable_internal(const char *name, char *value, t_hash *table, t_minishell *minishell)
{
	t_variable	*entry;
	t_bucket	*bucket;

	entry = NULL;
	bucket = hash_search(name, table);
	if (bucket)
		entry = bucket->data;
	if (entry)
	{
		entry = find_variable_internal(entry->name, minishell);
		if (!entry)
			return (entry);
	}
	else if (!entry)
	{
		entry = make_new_variable(name, table, minishell);
		entry->value = make_variable_value(value);
	}
	return (entry);
}

t_variable	*bind_variable(const char *name, char *value, t_minishell *minishell)
{
	t_variable	*v;
	t_context	*vc;
	t_context	*nvc;
	t_bucket	*b;

	if (!minishell->global_variables)
		create_variable_tables(minishell);
	vc = minishell->global_variables;
	while (vc)
	{
		v = NULL;
		b = hash_search(name, vc->table);
		if (b)
			v = b->data;
		nvc = vc;
		if (v)
			return (bind_variable_internal(v->name, value, nvc->table, minishell));
		vc = vc->down;
	}
	return (bind_variable_internal(name, value, minishell->global_variables->table, minishell));
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

int	path_isdir(char *path)
{
	int l;
	struct stat sb;

	errno = 0;
	l = stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
	return (l);
}

char	*sh_canonpath(char *path)
{
	char	stub_char;
	char	*result;
	char	*p;
	char	*q;
	char	*base;
	char	*dotdot;
	int		rooted;
	int		double_slash_path;
	char	c;

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
				if(path_isdir(result) == 0)
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
			if (path_isdir(result) == 0)
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

	home_var = find_variable_internal("HOME", minishell);
	home_string = home_var ? home_var->value : NULL;
	temp_var = find_variable_internal("PWD", minishell);
	if (temp_var)
		temp_string = temp_var->value;
	if (temp_var && temp_var->value && temp_string[0] == '/' && same_file(temp_string, ".", NULL, NULL))
	{
		current_dir = sh_canonpath(temp_string);
		if (!current_dir)
			current_dir = ft_strdup(getcwd(0,0));
		else
			temp_var = bind_variable("PWD", current_dir, minishell);
	}
	else if (home_string && same_file (home_string, ".", NULL, NULL))
		temp_var = bind_variable ("PWD", home_string, minishell);
	else
	{
		temp_string = ft_strdup(getcwd(0,0));
		if (temp_string)
			temp_var = bind_variable ("PWD", temp_string, minishell);
	}
	temp_var = find_variable_internal("OLDPWD", minishell);
	if (!temp_var || !temp_var->value)
		temp_var = bind_variable ("OLDPWD", NULL, minishell);
}

t_variable	*set_if_not (char *name, char *value, t_minishell *minishell)
{
	t_variable	*v;

	if (!minishell->global_variables)
		create_variable_tables(minishell);
	v = find_variable_internal(name, minishell);
	if (!v)
		v = bind_variable_internal(name, value, minishell->global_variables->table, minishell);
	return (v);
}

void	adjust_shell_level(int change, t_minishell *minishell)
{
	char		new_level[5];
	char		*old_SHLVL;
	long		old_level;
	int			shell_level;
	t_variable	*temp_var;

	temp_var = find_variable_internal("SHLVL", minishell);
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
	temp_var = bind_variable ("SHLVL", new_level, minishell);
}

void	set_home_var(t_minishell *minishell)
{
  t_variable	*temp_var;

	temp_var = find_variable_internal("HOME", minishell);
	if (!temp_var)
		temp_var = bind_variable ("HOME", ft_strdup("/"), minishell);
}

void	set_shell_var(t_minishell *minishell)
{
	t_variable	*temp_var;

	temp_var = find_variable_internal("SHELL", minishell);
	if (!temp_var)
		bind_variable ("SHELL", "/bin/sh", minishell);
}

void	initialize_shell_variables(char **env, t_minishell *minishell)
{
	char		*name;
	char		*string;
	int			c;
	int			char_index;
	int			string_index;
	t_variable	*temp_var;

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
		temp_var = NULL;
		if (legal_identifier(name))
			temp_var = bind_variable(name, string, minishell);
		name[char_index] = '=';
		temp_var->exportstr = ft_strdup(name);
	}
	set_pwd(minishell);
	temp_var = set_if_not("PATH", DEFAULT_PATH_VALUE, minishell);
	temp_var = set_if_not("TERM", "dumb", minishell);
	set_if_not("PS1", PS1, minishell);
	set_if_not("PS2", PS2, minishell);
	adjust_shell_level(1, minishell);
	set_home_var(minishell);
	set_shell_var(minishell);
}
