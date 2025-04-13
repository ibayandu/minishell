/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:47:08 by ibayandu          #+#    #+#             */
/*   Updated: 2024/10/12 12:11:47 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	s1len;
	size_t	s2len;
	size_t	total_len;

	s2len = ft_strlen(s2);
	s1len = ft_strlen(s1);
	if (!s1 || !s2)
		return (NULL);
	total_len = s1len + s2len;
	result = (char *)malloc((total_len * sizeof(char)) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, s1len + 1);
	ft_strlcpy((result + s1len), s2, s2len + 1);
	return (result);
}
