
#ifndef HELPER_H
# define HELPER_H

# include "lexer.h"
# include "libft.h"

int		match(t_list *tokens, t_token_type type);
t_token	*consume(t_list **tokens);

#endif // HELPER_H
