/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:28:30 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 16:42:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int	a;
	int	b;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n == -2147483648)
	{
		ft_putchar_fd('2', fd);
		n = 147483648;
	}
	b = n / 10;
	if (n > 9)
		ft_putnbr_fd(b, fd);
	a = (n % 10) + 48;
	ft_putchar_fd(a, fd);
}
