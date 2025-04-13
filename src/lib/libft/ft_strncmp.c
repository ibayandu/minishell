/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:21:50 by ibayandu          #+#    #+#             */
/*   Updated: 2024/10/12 10:22:43 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (*s1 && *s1 == *s2 && n > 0)
	{
		s1++;
		s2++;
		n--;
	}
	if (!n)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
