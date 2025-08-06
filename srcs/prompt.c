/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 06:29:32 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/05 12:38:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <x86_64-linux-gnu/bits/local_lim.h>
#include "libmem.h"
#include "minishell.h"
#include "expander.h"

#define ESC 27

// Ev dizinini '~' ile kısaltan yardımcı
static char	*get_short_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*home;
	size_t	home_len;
	char	*short_pwd;

	home = getenv("HOME");
	if (!home)
		home = "";
	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_strdup("?"));
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0)
	{
		short_pwd = mem_malloc(ft_strlen(cwd) - home_len + 2);
		if (!short_pwd)
			return (ft_strdup(cwd));
		short_pwd[0] = '~';
		ft_strlcpy(short_pwd + 1, cwd + home_len, ft_strlen(cwd + home_len - 1));
		return (short_pwd);
	}
	return (ft_strdup(cwd));
}

// Son klasör adını döner
static char	*get_basename(const char *path)
{
	char	*slash;
		char *tmp;
		char *res;

	if (!path)
		return (ft_strdup("?"));
	slash = ft_strrchr(path, '/');
	if (!slash)
		return (ft_strdup(path));
	else if (*(slash + 1) == '\0')
	{
		tmp = ft_strndup(path, slash - path);
		res = get_basename(tmp);
		return (res);
	}
	else
		return (ft_strdup(slash + 1));
}

// Tam hostname alır (FQDN değil, basit hostname)
static char	*get_hostname_full(void)
{
	char	hostname[HOST_NAME_MAX + 1];

	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (ft_strdup(hostname));
	return (ft_strdup("unknown"));
}

// Aktif kullanıcı adı
static char	*get_username(void)
{
	char			*user;
	struct passwd	*pw;

	user = getenv("USER");
	if (user)
		return (ft_strdup(user));
	pw = getpwuid(getuid());
	if (pw)
		return (ft_strdup(pw->pw_name));
	return (ft_strdup("unknown"));
}

// Kaçış dizilerini (\e gibi) gerçek karakterlere dönüştürür
static char	*replace_escape_sequences(const char *input)
{
	size_t	i;
	size_t	j;
	char	*result;

	result = mem_malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\\')
		{
			i++;
			if (input[i] == 'e')
			{
				result[j++] = ESC;
				i++;
			}
			else if (input[i] == '\\')
			{
				result[j++] = '\\';
				i++;
			}
			else if (input[i] == 'n')
			{
				result[j++] = '\n';
				i++;
			}
			else
			{
				result[j++] = '\\';
				if (input[i])
					result[j++] = input[i++];
			}
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*decode_prompt(const char *ps1)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*tmp;
	char	*decoded;
	char	*result;
	int		in_escape;
	char	*sub;
	char	*short_pwd;
	size_t	sub_len;

	if (!ps1)
		return (NULL);
	len = ft_strlen(ps1);
	tmp = mem_malloc(len + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	j = 0;
	in_escape = 0;
	// 1) \[ ... \] bloklarını kaldır (terminal dışı görünmez escape bloğu)
	while (ps1[i])
	{
		if (!in_escape)
		{
			if (ps1[i] == '\\' && ps1[i + 1] == '[')
			{
				in_escape = 1;
				i += 2;
			}
			else
				tmp[j++] = ps1[i++];
		}
		else
		{
			if (ps1[i] == '\\' && ps1[i + 1] == ']')
			{
				in_escape = 0;
				i += 2;
			}
			else
				tmp[j++] = ps1[i++];
		}
	}
	tmp[j] = '\0';
	// 2) Kaçış dizilerini gerçek karakterlere çevir
	decoded = replace_escape_sequences(tmp);
	if (!decoded)
		return (NULL);
	// 3) PS1 özel kısaltmalarını işleme
	result = mem_malloc(ft_strlen(decoded) * 4 + 1); // geniş yer ayırdık
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (decoded[i])
	{
		if (decoded[i] == '\\')
		{
			i++;
			sub = NULL;
			switch (decoded[i])
			{
			case 'u': // kullanıcı adı
				sub = get_username();
				break ;
			case 'H': // hostname tam
				sub = get_hostname_full();
				break ;
			case 'w': // tam yol, ~ ile kısaltılmış
				sub = get_short_pwd();
				break ;
			case 'W': // sadece son klasör adı
			{
				short_pwd = get_short_pwd();
				if (!short_pwd)
					sub = ft_strdup("?");
				else
					sub = get_basename(short_pwd);
				break ;
			}
			case 's': // shell adı
				sub = ft_strdup("minishell");
				break ;
			case '\\': // ters eğik çizgi
				result[j++] = '\\';
				i++;
				continue ;
			default:
				result[j++] = '\\';
				if (decoded[i])
					result[j++] = decoded[i++];
				else
					i++;
				continue ;
			}
			if (sub)
			{
				sub_len = ft_strlen(sub);
				ft_memcpy(result + j, sub, sub_len);
				j += sub_len;
			}
			if (decoded[i])
				i++;
		}
		else
			result[j++] = decoded[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*get_prompt(void)
{
	t_variable	*v;
	char		*ps1;

	ps1 = PS1;
	v = find_variable("PS1", create_tables(0));
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
	return (ps1);
}
