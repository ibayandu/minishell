/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:30 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 00:52:49 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

int		is_operator(char *str);
int		skip_whitespace_and_comments(const char *input, int *index,
			int *in_token);
char	*trim(char *str);
char	*ft_strdup(const char *str);

#endif // COMMON_H
