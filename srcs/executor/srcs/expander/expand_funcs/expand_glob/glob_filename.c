/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:21:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/11 17:24:18 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static char	**handle_empty_filename(char *directory_name, char **result)
{
	result = ft_realloc_vec(result, 2 * sizeof(char *));
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

char	**glob_filename(char *pathname, int flags)
{
	char			**result;
	int				result_size;
	char			*directory_name;
	char			*filename;
	char			*dname;
	int				directory_len;
	int				dflags;
	char			**directories;
	char			*d;
	char			*p;
	unsigned int	i;
	int				all_starstar;
	int				last_starstar;
	int				dl;
	int				prev;
	char			**temp_results;
	int				shouldbreak;
	int				n;
	char			**array;
	unsigned int	l;

	result = mem_malloc(sizeof(char *));
	result_size = 1;
	*result = NULL;
	directory_name = NULL;
	filename = ft_strrchr(pathname, '/');
	if (!filename)
	{
		filename = pathname;
		directory_name = "";
		directory_len = 0;
	}
	else
	{
		directory_len = (filename++ - pathname) + 1;
		directory_name = mem_calloc(directory_len + 1, 1);
		ft_memcpy(directory_name, pathname, directory_len);
	}
	if (directory_len > 0 && glob_pattern(directory_name) == 1)
	{
		all_starstar = 0;
		last_starstar = 0;
		d = directory_name;
		dflags = flags;
		if (*d == CTLESC && d[1] == CTLESC && (d[2] == '/' || !d[2]))
		{
			p = d;
			while (*d == CTLESC && d[1] == CTLESC && (d[2] == '/' || !d[2]))
			{
				p = d;
				if (d[2])
				{
					d += 3;
					while (*d == '/')
						d++;
					if (!*d)
						break ;
				}
			}
			all_starstar = 1 * !*d;
			d = p;
			dflags |= GX_ALLDIRS | GX_ADDCURDIR;
			directory_len = ft_strlen(d);
		}
		if (!all_starstar)
		{
			dl = directory_len;
			prev = dl;
			while (dl >= 4 && d[dl - 1] == '/' && d[dl - 2] == CTLESC
				&& d[dl - 3] == CTLESC && d[dl - 4] == '/')
			{
				prev = dl;
				dl -= 3;
			}
			last_starstar = 1 * dl != directory_len;
			directory_len = prev;
		}
		directory_len -= 3 * last_starstar && directory_len > 4
			&& !ft_strncmp(filename, DCTLESC, 2);
		if (d[directory_len - 1] == '/')
			d[directory_len - 1] = '\0';
		directories = glob_filename(d, dflags);
		if (ft_strrchr(pathname, '/'))
			directory_name = NULL;
		else if (!directories || !*directories)
			return (NULL);
		if (all_starstar && !ft_strncmp(filename, DCTLESC, 2) && !*filename)
			return (handle_empty_filename(NULL, result));
		else if (all_starstar && !ft_strncmp(filename, DCTLESC, 2))
			return (handle_only_filename(NULL, 0, filename, flags));
		i = -1;
		while (directories[++i])
		{
			shouldbreak = 0;
			dname = directories[i];
			dflags = flags & ~(GX_ALLDIRS | GX_ADDCURDIR);
			if (*filename == CTLESC && filename[1] == CTLESC
				&& !filename[2])
				dflags |= GX_ALLDIRS | GX_ADDCURDIR;
			if (!*dname && *filename)
			{
				dflags |= GX_NULLDIR;
				dname = ".";
			}
			if (all_starstar && !(dflags & GX_NULLDIR)
				&& testdir(dname) == -2 && !testdir(dname) && *filename)
				temp_results = NULL;
			else if (all_starstar && !(dflags & GX_NULLDIR)
				&& testdir(dname) == -2 && !testdir(dname))
				temp_results = handle_empty_filename("", NULL);
			else
				temp_results = glob_vector(filename, dname, dflags);
			if (temp_results)
			{
				if ((dflags & GX_ALLDIRS) && *filename == CTLESC
					&& filename[1] == CTLESC && (!filename[2]
						|| filename[2] == '/'))
				{
					if ((dflags & GX_NULLDIR) && !(flags & GX_NULLDIR)
						&& temp_results && *temp_results
						&& !**temp_results)
					{
						n = 0;
						while (temp_results[n] && *temp_results[n])
							n++;
						i = n;
						temp_results[i - n] = temp_results[i];
						while (temp_results[i++])
							temp_results[i - n] = temp_results[i];
						array = temp_results;
						shouldbreak = 1;
					}
					else
						array = temp_results;
				}
				else
					array = arraydir(directories[i], temp_results);
				result = ft_realloc_vec(result,
						(result_size + strvec_len(array)) * 8);
				l = -1;
				while (array[++l])
					result[result_size++ - 1] = array[l];
				result[result_size - 1] = NULL;
				if (shouldbreak)
					break ;
			}
		}
		return (result);
	}
	if (!*filename)
		return (handle_empty_filename(directory_name, result));
	return (handle_only_filename(directory_name, directory_len,
			filename, flags));
}
