/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:15:42 by ibayandu          #+#    #+#             */
/*   Updated: 2024/10/09 22:19:48 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	length;

	length = ft_strlen(s);
	while (length >= 0)
	{
		if (*(s + length) == (char)c)
			return ((char *)(s + length));
		length--;
	}
	return (0);
}
