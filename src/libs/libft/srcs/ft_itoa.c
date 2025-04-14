/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:14:55 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/14 21:07:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

static int	how_many_digits(long n)
{
	int	result;

	result = 1;
	if (n < 0)
	{
		n *= -1;
		result++;
	}
	while (n > 9)
	{
		n /= 10;
		result++;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	long	number;
	int		length_number;
	char	*result;
	int		is_minus;

	is_minus = 0;
	number = (long)n;
	length_number = how_many_digits(number);
	if (number < 0)
	{
		is_minus = 1;
		number *= -1;
	}
	result = ft_calloc(length_number + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (length_number--)
	{
		result[length_number] = (number % 10) + '0';
		number /= 10;
	}
	if (is_minus)
		result[0] = '-';
	return (result);
}
