/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:45:57 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/20 18:44:48 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAGS_H
# define FLAGS_H

// Quote Handling
# define Q_DOUBLE_QUOTES  0x001
# define Q_HERE_DOCUMENT  0x002

// Character Flags
# define CBSDQUOTE 0x0040

// Command Flags
# define CMD_WANT_SUBSHELL 0x01

#endif // FLAGS_H
