/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/21 21:50:57 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdio.h>

int	main(void)
{
	t_list	*tokens;
	t_list	*current;
	t_token	*token;

	char *input = "find /home/user/Documents -type f -name \"*.txt\" -exec grep\
		-i \"error\" {} \\; | sort | uniq -c | tail -n 10 | awk '{print $2 \" - \" $1 \" occurrences \"}' > top_errors.txt";
		tokens = lexer(input);
	current = tokens;
	while (current)
	{
		token = (t_token *)current->content;
		printf("Token Type: %d, Value: '%s'\n", token->token_type,
			token->value);
		current = current->next;
	}
	ft_free();
	return (0);
}
