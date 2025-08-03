/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:47:28 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:26:59 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "builtin.h"
# include "expander.h"

size_t	count_strings(char **arr);
char	**strvec_sort(char **input, int is_asc);

#endif // EXPORT_H
