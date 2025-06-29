#include "minishell.h"
#include "libft.h"

t_var_list	*vlist_alloc(int nentries)
{
	t_var_list	*vlist;

	vlist = ft_malloc(sizeof(t_var_list));
	vlist->list = ft_malloc((nentries + 1) * sizeof(t_variable *));
	vlist->list_size = nentries;
	vlist->list_len = 0;
	vlist->list[0] = NULL;
	return (vlist);
}

t_var_list	*vlist_realloc(t_var_list *vlist, int n)
{
	if (!vlist)
		return (vlist = vlist_alloc(n));
	if (n > vlist->list_size)
	{
		vlist->list_size = n;
		vlist->list = ft_realloc(vlist->list, (vlist->list_size + 1) * sizeof(t_variable *));
	}
	return (vlist);
}

void	vlist_add(t_var_list *vlist, t_variable *var)
{
	int	i;

	i = -1;
	while (++i < vlist->list_len)
		if (ft_strncmp(var->name, vlist->list[i]->name, ft_strlen(vlist->list[i]->name)) == 0)
			break ;
	if (i < vlist->list_len)
		return ;
	if (i >= vlist->list_len)
		vlist = vlist_realloc(vlist, vlist->list_size + 16);
	vlist->list[vlist->list_len++] = var;
	vlist->list[vlist->list_len] = NULL;
}

void	flatten(t_hash *var_hash_table, int (*func)(t_variable *), t_var_list *vlist)
{
	int			i;
	t_bucket	*tlist;
	int			r;
	t_variable	*var;

	if (!var_hash_table || !var_hash_table->nentries || (!vlist && !func))
		return ;
	i = -1;
	while (++i < var_hash_table->nbuckets)
	{
		tlist = var_hash_table && i < var_hash_table->nbuckets ? var_hash_table->bucket_array[i] : NULL;
		while (tlist)
		{
			var = tlist->data;
			r = func ? (*func)(var) : 1;
			if (r && vlist)
				vlist_add(vlist, var);
			tlist = tlist->next;
		}
	}
}

t_variable	**map_over(int (*function)(t_variable *), t_hash *ht)
{
	t_hash		*h;
	t_var_list	*vlist;
	t_variable	**ret;
	int			nentries;

	nentries = 0;
	h = ht;
	if (h)
		nentries += h ? h->nentries : 0;
	if (!nentries)
		return (NULL);
	vlist = vlist_alloc(nentries);
	h = ht;
	if (h)
		flatten(h, function, vlist);
	ret = vlist->list;
	return (ret);
}

int	export_environment_candidate(t_variable *var)
{
	if (var)
		return (1);
	return (0);
}

int	strvec_len(char **array)
{
	int	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	*mk_env_string(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*p;
	char	*q;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	p = ft_malloc(2 + name_len + value_len);
	ft_memcpy(p, name, name_len);
	q = p + name_len;
	if (value)
		q[0] = '=';
	if (value && *value)
		ft_memcpy(q + 1, value, value_len + 1);
	else
		q[1 - (!value)] = '\0';
	return (p);
}

char	**make_env_array_from_var_list(t_variable **vars, int is_export)
{
	int			i;
	int			list_index;
	t_variable	*var;
	char		**list;
	char		*value;

	list = ft_malloc((1 + strvec_len((char **)vars)) * sizeof(char *));
	i = 0;
	list_index = 0;
	var = vars[i];
	while (var)
	{
		value = var->value;
		if (value || is_export)
		{
			list[list_index] = mk_env_string(var->name, value);
			list_index++;
		}
		i++;
		var = vars[i];
	}
	list[list_index] = NULL;
	return (list);
}

char	**make_var_export_array(t_hash *ht, int is_export)
{
	char		**list;
	t_variable	**vars;

	vars = map_over(export_environment_candidate, ht);
	if (!vars)
		return (NULL);
	list = make_env_array_from_var_list(vars, is_export);
	return (list);
}
