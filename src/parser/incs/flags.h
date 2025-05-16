/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:45:57 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/16 23:39:00 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAGS_H
# define FLAGS_H

// Word Flags
# define W_HASDOLLAR (1 << 0)
# define W_QUOTED (1 << 1)

// Parser States
# define PST_REDIRLIST 0x080000

// Quote Handling
# define Q_DOUBLE_QUOTES  0x001
# define Q_HERE_DOCUMENT  0x002

// Character Flags
# define CWORD 0x0000	/* nothing special; an ordinary character */
# define CSHMETA 	0x0001	/* shell meta character */
# define CSHBRK 0x0002	/* shell break character */
# define CBACKQ 0x0004	/* back quote */
# define CQUOTE 0x0008	/* shell quote character */
# define CSPECL 0x0010	/* special character that needs quoting */
# define CEXP 0x0020	/* shell expansion character */
# define CBSDQUOTE 0x0040	/* characters escaped by backslash in double quotes */
# define CBSHDOC 0x0080	/* characters escaped by backslash in here doc */
# define CGLOB 0x0100	/* globbing characters */
# define CXGLOB 0x0200	/* extended globbing characters */
# define CXQUOTE 0x0400	/* cquote + backslash */
# define CSPECVAR 0x0800	/* single-character shell variable name */
# define CSUBSTOP 0x1000	/* values of OP for ${word[:]OPstuff} */
# define CBLANK 0x2000	/* whitespace (blank) character */

#endif // FLAGS_H
