/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 20:42:10 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "lexer.h"
#include "token_utils.h"

static int	is_quote_closed(char *input)
{
	int	single_quote;
	int	double_quote;

	single_quote = 0;
	double_quote = 0;
	while (*input)
	{
		if (*input == '"' && !single_quote)
			double_quote = !double_quote;
		else if (*input == '\'' && !double_quote)
			single_quote = !single_quote;
		input++;
	}
	return (single_quote == 0 && double_quote == 0);
}

char	*get_quote_str(char *token)
{
	char	*quote;

	quote = ft_calloc(sizeof(char), 2);
	while (*token)
	{
		if (*token == '"' && quote[0] != '"')
			quote[0] = '"';
		else if (*token == '\'' && quote[0] != '\'')
			quote[0] = '\'';
		token++;
	}
	return (quote);
}

/**
 * @brief kullanıcıdan alınan input'u bu fonksiyona verdiğinde senin için
 * tokenize eder.
 * @param input tokenize edilecek string.
 * @return girdiyi tiplerine göre ayrıştırarak t_list* döner.
 * @note t_token türüne incs klasörü altında token.h dosyasının
 * içinde ulaşabilirsin.
 *
 * Örnek Kullanım
 *
 * t_token	x = lexer(input);
 */
static t_list	*lexer(char *input)
{
	t_list	*tokens;
	t_token	*token;
	char	*errmsg;

	tokens = NULL;
	while (*input)
	{
		if (ft_isspace(*input))
		{
			input++;
			continue ;
		}
		token = create_token(get_token_type(input), get_token_str(input));
		if (!is_quote_closed(token->value))
		{
			errmsg = ft_strjoin("minishell: syntax error near unexpected token `",
					get_quote_str(token->value));
			errmsg = ft_strjoin(errmsg, "'");
			ft_putendl_fd(errmsg, 2);
			return (NULL);
		}
		if (token)
		{
			ft_lstadd_back(&tokens, ft_lstnew(token));
			input += ft_strlen(token->value);
		}
	}
	((t_token *)(ft_lstlast(tokens)->content))->flags = 1;
	return (tokens);
}

/**
 * @brief Basit bir lexer başlatıcı ve iterator işlevi.
 *
 * input parametresi NULL değilse, verilen girdi stringini tokenize eder
 * ve oluşan token listesinin başlangıcını döner. input NULL ise, daha önce
 * oluşturulmuş token listesinden bir sonraki token'ı döner.
 *
 * @param input Tokenize edilecek giriş metni.
 * NULL ise mevcut listedeki sonraki token döner.
 * @return void* Token listesinde geçerli düğüm (t_list*).
 * Liste sonuna gelindiyse NULL döner.
 */
void	*init_lexer(char *input)
{
	static t_list	*tokenlist;

	if (input)
	{
		tokenlist = lexer(input);
		return (tokenlist);
	}
	return (&tokenlist);
}

/**
 * @brief Bir sonraki token'ı döner.
 *
 * Önceden başlatılmış token listesinden sıradaki token'ı alır.
 * Token listesi `init_lexer()` aracılığıyla başlatılmış olmalıdır.
 * Her çağrıda listeyi bir adım ilerletir ve geçerli token'ı döner.
 *
 * @return t_token* Geçerli token nesnesi. Token kalmamışsa NULL döner.
 */
t_token	*get_next_token(void)
{
	t_list	**token_list;

	token_list = init_lexer(NULL);
	if (!token_list)
		return (NULL);
	if ((*token_list)->next)
		*token_list = (*token_list)->next;
	return ((t_token *)(*token_list)->content);
}

t_token	*get_current_token(void)
{
	t_list	**token_list;

	token_list = init_lexer(NULL);
	if (!token_list)
		return (NULL);
	if (!(*token_list))
		return (NULL);
	return ((t_token *)(*token_list)->content);
}

int	consume_token(t_token_type type)
{
	if (type == get_current_token()->token_type)
	{
		get_next_token();
		return (1);
	}
	else
		return (0);
}
