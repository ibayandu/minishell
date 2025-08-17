/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:22:32 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:38:03 by yzeybek          ###   ########.tr       */
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

static int	init_vector(t_glob_state *const *gs, t_list **lastlink, char *pat,
	char *dir)
{
	DIR			*d;
	struct stat	finfo;

	*lastlink = NULL;
	(*gs)->dirname = dir;
	(*gs)->filename = pat;
	(*gs)->dirlen = 0;
	d = opendir((*gs)->dirname);
	if (((!(*gs)->filename || !*(*gs)->filename
				|| !glob_pattern((*gs)->filename))
			&& testdir((*gs)->dirname) < 0)
		|| (glob_pattern((*gs)->filename) && !d))
		return (1);
	if (!((*gs)->filename) || !*((*gs)->filename))
		*lastlink = make_node(NULL, ft_strdup(""), &((*gs)->dirlen));
	else if (!(!glob_pattern((*gs)->filename)
			&& lstat(ft_strjoin(ft_strjoin((*gs)->dirname, "/"),
					(*gs)->filename), &finfo))
		&& glob_pattern((*gs)->filename))
		loop_dir((*gs), d, lastlink);
	closedir(d);
	return (0);
}

char	**glob_vector(char *pat, char *dir, int flags)
{
	t_glob_state *const	glob_state = mem_malloc(sizeof(t_glob_state));
	t_list				*lastlink;
	char				**name_vector;
	t_list				*tmplink;

	glob_state->flags = flags;
	if (init_vector(&glob_state, &lastlink, pat, dir))
		return (NULL);
	if ((glob_state->flags & (GX_ALLDIRS | GX_ADDCURDIR)) == (GX_ALLDIRS
			| GX_ADDCURDIR) && (glob_state->flags & GX_NULLDIR))
		lastlink = make_node(lastlink, ft_strdup(""), &glob_state->dirlen);
	else if ((glob_state->flags & (GX_ALLDIRS | GX_ADDCURDIR))
		== (GX_ALLDIRS | GX_ADDCURDIR))
		lastlink = make_node(lastlink, ft_strdup(glob_state->dirname),
				&glob_state->dirlen);
	name_vector = mem_malloc((glob_state->dirlen + 1) * sizeof(char *));
	tmplink = lastlink;
	glob_state->is_all_dctlesc = -1;
	while (++glob_state->is_all_dctlesc < glob_state->dirlen)
	{
		name_vector[glob_state->is_all_dctlesc] = tmplink->content;
		tmplink = tmplink->next;
	}
	name_vector[glob_state->dirlen] = NULL;
	return (name_vector);
}
