/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 13:09:10 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"
#include "lib/libft/libft.h"

int	main(void)
{
	char	*input;
	t_list	*token_list;

	input = "avccsadasdasd";
	token_list = lexer(input);
}
