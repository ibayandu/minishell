/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/12 21:35:32 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

int	main(void)
{
	char			*input;
	t_token_list	*token_list;

	input = "avccsadasdasd";
	token_list = lexer(input);
	while (token_list->tokens->next)
	{
		ft_putendl_fd((char *)token_list->tokens->content, 1);
		ft_putendl_fd("", 1);
		token_list->tokens = token_list->tokens->next;
	}
	ft_putnbr_fd(token_list->count, 1);
	ft_putendl_fd("", 1);
}
