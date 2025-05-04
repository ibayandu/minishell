/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 02:08:13 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdio.h>

const char	*token_type_to_string(t_token_type type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_LESS)
		return ("T_LESS");
	else if (type == T_GREAT)
		return ("T_GREAT");
	else if (type == T_DLESS)
		return ("T_DLESS");
	else if (type == T_DGREAT)
		return ("T_DGREAT");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_AND_IF)
		return ("T_AND_IF");
	else if (type == T_OR_IF)
		return ("T_OR_IF");
	else if (type == T_LPARANTHESE)
		return ("T_LPARANTHESE");
	else if (type == T_RPARANTHESE)
		return ("T_RPARANTHESE");
	else if (type == T_EOF)
		return ("T_EOF");
	else
		return ("UNKNOWN");
}

#define blue "\033[0;34m"
#define yellow "\033[0;33m"
#define green "\033[0;32m"
#define defaultcolor "\033[0;0m"

int	main(void)
{
	t_list	*tokens;
	t_list	*current;
	t_token	*token;
	char	*input;

	input = "( echo \"Başlangiç: $(date)\"\
		&& cat <<EOF; echo \"Heredoc içerik\"; EOF )\
		&& echo \"Alt işlem başarıli\"\
		|| echo \"Alt işlem başarısız\" | grep \"içerik\"\
		|| echo \"Eşleşme bulunamadı\" > output.txt && cat < output.txt\
		|| cat <<EOF > new_output.txt; echo \"Yeni dosyaya yazılan içerik\"; EOF";
	tokens = lexer(input);
	current = tokens;
	while (current)
	{
		token = (t_token *)current->content;
		printf(yellow"Token Type: "blue"%-13s "yellow"Value: "green"%s"defaultcolor"\n",
			token_type_to_string(token->token_type),token->value);
		current = current->next;
	}
	ft_free();
	return (0);
}
