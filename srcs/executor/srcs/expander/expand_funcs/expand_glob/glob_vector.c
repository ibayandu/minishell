/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:22:32 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/13 22:02:45 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "libmem.h"
#include "expander.h"

static t_list	*make_node(t_list *next, void *content, int *count)
{
	t_list	*res;

	res = mem_malloc(sizeof(t_list));
	res->next = next;
	res->content = content;
	*count += 1;
	return (res);
}

static int	has_dot_or_not(char *pat, struct dirent *dp, int not, int flags)
{
	if (!not)
		return ((*pat == '.' || *(dp->d_name) != '.' || (dp->d_name[1]
					&& ft_strcmp(dp->d_name + 1, "."))) && (flags
				& GX_ALLDIRS));
	return ((*pat == '.' || *(dp->d_name) != '.' || (dp->d_name[1]
				&& ft_strcmp(dp->d_name + 1, "."))) && (ft_strchr(pat, '.')
			|| (ft_strcmp(dp->d_name, ".") && ft_strcmp(dp->d_name, "..")))
		&& (*(dp->d_name) != '.' || *pat == '.')
		&& !(flags & GX_ALLDIRS) && !glob_match(pat, dp->d_name));
}

static void	loop_dir(t_glob_state *gs, DIR *d, t_list **lastlink)
{
	struct dirent	*dp;
	char			*subdir;

	while (1)
	{
		dp = readdir(d);
		if (!dp)
			break ;
		if (has_dot_or_not(gs->filename, dp, 0, gs->flags))
		{
			gs->is_all_dctlesc = 0 + (gs->flags & GX_ALLDIRS) * MP_RMDOT;
			if (gs->flags & GX_NULLDIR)
				gs->is_all_dctlesc |= MP_IGNDOT;
			subdir = glob_makepath(gs->dirname, dp->d_name,
					gs->is_all_dctlesc);
			if (!testdir(subdir))
				finddir(glob_vector(gs->filename, subdir,
						(gs->flags & ~GX_ADDCURDIR)), &gs->dirlen, lastlink);
			*lastlink = make_node(*lastlink, ft_strdup(subdir), &gs->dirlen);
		}
		if (has_dot_or_not(gs->filename, dp, 1, gs->flags))
			*lastlink = make_node(*lastlink, ft_strdup(dp->d_name),
					&gs->dirlen);
	}
}

char	**glob_vector(char *pat, char *dir, int flags)
{
	DIR				*d;
	t_list			*lastlink;
	struct stat		finfo;
	char			**name_vector;
	int				i;
	t_list			*tmplink;
	t_glob_state	*glob_state;

	lastlink = NULL;
	glob_state = mem_malloc(sizeof(t_glob_state));
	glob_state->dirname = dir;
	glob_state->filename = pat;
	glob_state->flags = flags;
	glob_state->dirlen = 0;
	name_vector = NULL;
	d = opendir(glob_state->dirname);
	if (((!glob_state->filename || !*glob_state->filename
				|| !glob_pattern(glob_state->filename))
			&& testdir(glob_state->dirname) < 0)
		|| (glob_pattern(glob_state->filename) && !d))
		return (NULL);
	if (!glob_state->filename || !*glob_state->filename)
		lastlink = make_node(NULL, ft_strdup(""), &glob_state->dirlen);
	else if (!glob_pattern(glob_state->filename)
		&& lstat(ft_strjoin(ft_strjoin(glob_state->dirname, "/"),
				glob_state->filename), &finfo))
		lastlink = make_node(NULL, ft_strdup(glob_state->filename),
				&glob_state->dirlen);
	else if (glob_pattern(glob_state->filename))
		loop_dir(glob_state, d, &lastlink);
	closedir(d);
	if ((glob_state->flags & (GX_ALLDIRS | GX_ADDCURDIR))
		== (GX_ALLDIRS | GX_ADDCURDIR)
		&& (glob_state->flags & GX_NULLDIR))
		lastlink = make_node(lastlink, ft_strdup(""), &glob_state->dirlen);
	else if ((glob_state->flags & (GX_ALLDIRS | GX_ADDCURDIR))
		== (GX_ALLDIRS | GX_ADDCURDIR))
		lastlink = make_node(lastlink, ft_strdup(glob_state->dirname),
				&glob_state->dirlen);
	name_vector = mem_malloc((glob_state->dirlen + 1) * sizeof(char *));
	tmplink = lastlink;
	i = -1;
	while (++i < glob_state->dirlen)
	{
		name_vector[i] = tmplink->content;
		tmplink = tmplink->next;
	}
	name_vector[glob_state->dirlen] = NULL;
	return (name_vector);
}
