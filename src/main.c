/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 21:10:03 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdio.h>

const char *token_type_to_string(t_token_type type)
{
    switch (type)
    {
    case T_WORD: return "T_WORD";
    case T_ASSIGNMENT_WORD: return "T_ASSIGNMENT_WORD";
    case T_LESS: return "T_LESS";
    case T_GREAT: return "T_GREAT";
    case T_DLESS: return "T_DLESS";
    case T_DGREAT: return "T_DGREAT";
    case T_SQUOTE: return "T_SQUOTE";
    case T_DQUOTE: return "T_DQUOTE";
    case T_PIPE: return "T_PIPE";
    case T_AND_IF: return "T_AND_IF";
    case T_OR_IF: return "T_OR_IF";
    case T_LPARANTHESE: return "T_LPARANTHESE";
    case T_RPARANTHESE: return "T_RPARANTHESE";
    case T_EOF: return "T_EOF";
    default: return "UNKNOWN";
    }
}


int	main(void)
{
	t_list	*tokens;
	t_list	*current;
	t_token	*token;

	char *input = "cat << EOF | grep \"ER'RO'R\" && e\"\"\"\"\"\"c''''ho \"Error found\" || echo \"No errors\" >> log.txt && tail -n 50 < system.log | sort | awk '{print $2 \" - \" $1 \" occurrences \"}' > top_errors.txt ";
	// char *input = "l's' ";
	tokens = lexer(input);  // Lexer fonksiyonunun tokenları döndürdüğünü varsayıyoruz
    current = tokens;

    // Token'ları yazdırma
    while (current)
    {
        token = (t_token *)current->content;
        printf("Token Type: %s, Value: %s\n", token_type_to_string(token->token_type), token->value);
        current = current->next;
    }

    ft_free();  // Burada kullanılan ft_free fonksiyonu da belleği temizlemelidir
    return (0);
}
