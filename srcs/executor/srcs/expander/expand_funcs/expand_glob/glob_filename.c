/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:21:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 22:10:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static char	**handle_empty_filename(char *directory_name, char **result)
{
	result = strvec_realloc(result, 2 * sizeof(char *));
	*result = ft_strdup(directory_name);
	result[1] = NULL;
	return (result);
}

static char	**handle_only_filename(char *directory_name, int directory_len,
	char *filename, int flags)
{
	char	**result;
	int		dflags;
	char	**temp_results;

	dflags = flags;
	if (!directory_len)
		dflags |= GX_NULLDIR;
	if (!ft_strncmp(filename, DCTLESC, 2) && filename[2] != CTLESC)
	{
		dflags |= GX_ALLDIRS | GX_ADDCURDIR;
		if (!directory_len && !(flags & GX_ALLDIRS))
			dflags &= ~GX_ADDCURDIR;
	}
	if (!directory_len)
		temp_results = glob_vector(filename, ".", dflags);
	else
		temp_results = glob_vector(filename, directory_name, dflags);
	if (!temp_results)
		return (NULL);
	if (dflags & GX_ALLDIRS)
		result = arraydir("", temp_results);
	else
		result = arraydir(directory_name, temp_results);
	return (result);
}

static void	all_dctlesc(t_glob_state *glob_state, int flags)
{
	char	*p;

	glob_state->flags = flags;
	if (!ft_strncmp(glob_state->filename, DCTLESC, 2)
		&& (glob_state->dirname[2] == '/' || !glob_state->dirname[2]))
	{
		p = glob_state->dirname;
		while (!ft_strncmp(glob_state->filename, DCTLESC, 2)
			&& (glob_state->dirname[2] == '/' || !glob_state->dirname[2]))
		{
			p = glob_state->dirname;
			if (glob_state->dirname[2])
			{
				glob_state->dirname += 3;
				while (*glob_state->dirname == '/')
					glob_state->dirname++;
				if (!*glob_state->dirname)
					break ;
			}
		}
		glob_state->is_all_dctlesc = !*glob_state->dirname;
		glob_state->dirname = p;
		glob_state->flags |= GX_ALLDIRS | GX_ADDCURDIR;
		glob_state->dirlen = ft_strlen(glob_state->dirname);
	}
}

static void	last_dctlesc(t_glob_state *glob_state)
{
	int		last_starstar;
	int		dl;
	int		prev;

	last_starstar = 0;
	if (!glob_state->is_all_dctlesc)
	{
		dl = glob_state->dirlen;
		prev = dl;
		while (dl >= 4 && glob_state->dirname[dl - 1] == '/'
			&& glob_state->dirname[dl - 2] == CTLESC
			&& glob_state->dirname[dl - 3] == CTLESC
			&& glob_state->dirname[dl - 4] == '/')
		{
			prev = dl;
			dl -= 3;
		}
		last_starstar = dl != glob_state->dirlen;
		glob_state->dirlen = prev;
	}
	glob_state->dirlen -= 3 * last_starstar && glob_state->dirlen > 4
		&& !ft_strncmp(glob_state->filename, DCTLESC, 2);
	if (glob_state->dirname[glob_state->dirlen - 1] == '/')
		glob_state->dirname[glob_state->dirlen - 1] = '\0';
}

static int	glob_should(t_glob_state *glob_state, int flags, char **temp_results)
{
	return (glob_state->flags & GX_NULLDIR) && !(flags & GX_NULLDIR)
		&& temp_results && *temp_results
		&& !**temp_results && (glob_state->flags & GX_ALLDIRS)
		&& !ft_strncmp(glob_state->filename, DCTLESC, 2)
		&& (!glob_state->filename[2] || glob_state->filename[2] == '/');
}

static int	glob_dir(t_glob_state *glob_state, char *dir, int flags, char ***result)
{
	char	**temp_results;
	char	**array;

	glob_state->flags = flags & ~(GX_ALLDIRS | GX_ADDCURDIR);
	if (!ft_strncmp(glob_state->filename, DCTLESC, 2)
		&& !glob_state->filename[2])
		glob_state->flags |= GX_ALLDIRS | GX_ADDCURDIR;
	if (!*(dir) && *glob_state->filename)
	{
		glob_state->flags |= GX_NULLDIR;
		dir = ".";
	}
	if (glob_state->is_all_dctlesc && !(glob_state->flags & GX_NULLDIR)
		&& testdir(dir) == -2 && !testdir(dir) && *glob_state->filename)
		temp_results = NULL;
	else if (glob_state->is_all_dctlesc && !(glob_state->flags & GX_NULLDIR)
		&& testdir(dir) == -2 && !testdir(dir))
		temp_results = handle_empty_filename("", NULL);
	else
		temp_results = glob_vector(glob_state->filename, dir,
			glob_state->flags);
	if (glob_should(glob_state, flags, temp_results))
		array = strvec_shift(temp_results);
	else if ((glob_state->flags & GX_ALLDIRS) && !ft_strncmp(
			glob_state->filename, DCTLESC, 2) && (!glob_state->filename[2]
			|| glob_state->filename[2] == '/') && temp_results)
		array = temp_results;
	else if (temp_results)
		array = arraydir(dir, temp_results);
	if (temp_results)
		*result = strvec_cpy(strvec_realloc(*result,
			strvec_len(*result) + strvec_len(array))
			+ strvec_len(*result), array) - strvec_len(*result);
	if (glob_should(glob_state, flags, temp_results))
		return (0);
	return (1);
}

static char	**glob_dirs(t_glob_state *glob_state, char *dirname, int dirlen, int flags)
{
	char	**result;
	char	**dirs;
	size_t	i;

	result = mem_calloc(sizeof(char *), 1);
	glob_state->dirname = dirname;
	glob_state->dirlen = dirlen;
	glob_state->is_all_dctlesc = 0;
	all_dctlesc(glob_state, flags);
	last_dctlesc(glob_state);
	dirs = glob_filename(glob_state->dirname, glob_state->flags);
	if (!dirs || !*dirs)
		return (NULL);
	if (glob_state->is_all_dctlesc && !ft_strncmp(glob_state->filename, DCTLESC, 2) && !*glob_state->filename)
		return (handle_empty_filename(NULL, result));
	else if (glob_state->is_all_dctlesc && !ft_strncmp(glob_state->filename, DCTLESC, 2))
		return (handle_only_filename(NULL, 0, glob_state->filename, flags));
	i = -1;
	while (dirs[++i] && glob_dir(glob_state, dirs[i], flags, &result))
		;
	return (result);
}

char	**glob_filename(char *pathname, int flags)
{
	t_glob_state	*glob_state;
	char			*dirname;
	int				dirlen;

	glob_state = mem_malloc(sizeof(t_glob_state));
	glob_state->filename = pathname;
	dirlen = 0;
	dirname = "";
	if(ft_strrchr(pathname, '/'))
	{
		glob_state->filename = ft_strrchr(pathname, '/');
		dirlen = (glob_state->filename++ - pathname) + 1;
		dirname = ft_strndup(pathname, dirlen);
	}
	if (dirlen > 0 && glob_pattern(dirname))
		return (glob_dirs(glob_state, dirname, dirlen, flags));
	if (!*glob_state->filename)
		return (handle_empty_filename(dirname, mem_calloc(sizeof(char *), 1)));
	return (handle_only_filename(dirname, dirlen,
			glob_state->filename, flags));
}
