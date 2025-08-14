/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:21:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 00:15:05 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

char	**glob_filename_empty(char *directory_name, char **result)
{
	result = strvec_realloc(result, 2 * sizeof(char *));
	*result = ft_strdup(directory_name);
	result[1] = NULL;
	return (result);
}

char	**glob_filename_only(char *directory_name, int directory_len,
	char *filename, int flags)
{
	char	**result;
	int		dflags;
	char	**temp_results;

	dflags = flags;
	if (!directory_len)
		dflags |= GX_NULLDIR;
	if (!ft_strncmp(filename, DCTLESC, 2) && (filename[2] == '/'
		|| !filename[2]))
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

char	**glob_filename(char *pathname, int flags)
{
	t_glob_state	*glob_state;
	char			*dirname;
	int				dirlen;

	glob_state = mem_malloc(sizeof(t_glob_state));
	glob_state->filename = pathname;
	dirlen = 0;
	dirname = "";
	if (ft_strrchr(pathname, '/'))
	{
		glob_state->filename = ft_strrchr(pathname, '/');
		dirlen = (glob_state->filename++ - pathname) + 1;
		dirname = ft_strndup(pathname, dirlen);
	}
	if (dirlen > 0 && glob_pattern(dirname))
		return (glob_dirs(glob_state, dirname, dirlen, flags));
	if (!*glob_state->filename)
		return (glob_filename_empty(dirname, mem_calloc(sizeof(char *), 1)));
	return (glob_filename_only(dirname, dirlen,
			glob_state->filename, flags));
}
