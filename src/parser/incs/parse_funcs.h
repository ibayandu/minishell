#ifndef PARSE_FUNCS_H
# define PARSE_FUNCS_H

# include "ast.h"
# include "helper.h"

t_ast	*parse_sequence(t_list **tokens);
t_ast	*parse_pipe(t_list **tokens);
t_ast	*parse_command(t_list **tokens);
t_redir	*parse_redirs(t_list **tokens);

#endif // PARSE_FUNCS_H
