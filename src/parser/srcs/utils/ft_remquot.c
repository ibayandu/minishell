/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/17 00:06:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "flags.h"

char	*string_extract_single_quoted (string, sindex, allowesc)
     char *string;
     int *sindex;
     int allowesc;
{
  register int i;
  size_t slen;
  char *t;
  int pass_next;
  DECLARE_MBSTATE;

  /* Don't need slen for ADVANCE_CHAR unless multibyte chars possible. */
  slen = (MB_CUR_MAX > 1) ? strlen (string + *sindex) + *sindex : 0;
  i = *sindex;
  pass_next = 0;
  while (string[i])
    {
      if (pass_next)
	{
	  pass_next = 0;
	  ADVANCE_CHAR (string, slen, i);
	  continue;
	}
      if (allowesc && string[i] == '\\')
	pass_next++;
      else if (string[i] == '\'')
        break;
      ADVANCE_CHAR (string, slen, i);
    }

  t = substring (string, *sindex, i);

  if (string[i])
    i++;
  *sindex = i;

  return (t);
}

char	*string_quote_removal(char *string, int quoted)
{
	size_t			slen;
	char			*r;
	char			*result_string;
	char			*temp;
	char			*send;
	int				sindex;
	int				tindex;
	int				dquote;
	unsigned char	c;

	slen = ft_strlen(string);
	send = string + slen;
	result_string = ft_malloc(slen + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	while (string[sindex])
	{
		c = string[sindex];
		if (c == '\\')
		{
			c = string[++sindex];
			if (c == 0)
			{
				*r++ = '\\';
			}
			else if (((quoted & (Q_HERE_DOCUMENT | Q_DOUBLE_QUOTES)) || dquote) && ft_charflag(c, CBSDQUOTE))
				*r++ = '\\';
		}
		else if (c == '\'')
		{
			if ((quoted & (Q_DOUBLE_QUOTES | Q_DOUBLE_QUOTES)) || dquote)
			{
				*r++ = c;
				sindex++;
			}
			else
			{
				tindex = sindex + 1;
				temp = string_extract_single_quoted(string, &tindex, 0);
			}
		}
		else
		{
			*r++ = string[sindex++];
			sindex++;
		}
	}
	for (dquote = sindex = 0; c = string[sindex];)
    {
      switch (c)
	{
	case '\\':
	  c = string[++sindex];
	  if (c == 0)
	    {
	      *r++ = '\\';
	      break;
	    }
	  if (((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || dquote) && ft_charflag(c, CBSDQUOTE))
	    *r++ = '\\';
	  /* FALLTHROUGH */

	default:
	  SCOPY_CHAR_M (r, string, send, sindex);
	  break;

	case '\'':
	  if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || dquote)
	    {
	      *r++ = c;
	      sindex++;
	      break;
	    }
	  tindex = sindex + 1;
	  temp = string_extract_single_quoted (string, &tindex, 0);
	  if (temp)
	    {
	      strcpy (r, temp);
	      r += strlen (r);
	      free (temp);
	    }
	  sindex = tindex;
	  break;

	case '"':
	  dquote = 1 - dquote;
	  sindex++;
	  break;
	}
    }
    *r = '\0';
    return (result_string);
}
