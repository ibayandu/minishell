%token <word> T_WORD
%token T_AND_IF T_OR_IF T_GREATER_GREATER T_LESS_LESS T_NUMBER_GREATER T_NUMBER_LESS T_NUMBER_GREATER_GREATER T_NUMBER_LESS T_NUMBER_LESS_LESS T_NL T_PIPE T_LPARANTHESE T_RPARANTHESE T_LESS T_GREAT
%type <command> inputunit command pipeline list simple_command compound_list
%type <redirect> redirection redirection_list
%type <element> simple_command_element
%start inputunit
%left T_NL
%left T_AND_IF T_OR_IF
%right T_PIPE
%%
inputunit:	newline_list list T_NL
|	newline_list
|	newline_list error T_NL
;
redirection:	T_GREAT T_WORD
|	T_LESS T_WORD
|	T_NUMBER_GREATER T_WORD
|	T_NUMBER_LESS T_WORD
|	T_GREATER_GREATER T_WORD
|	T_NUMBER_GREATER_GREATER T_WORD
|	T_LESS_LESS T_WORD
|	T_NUMBER_LESS_LESS T_WORD
;
simple_command_element: T_WORD
|	redirection
;
redirection_list: redirection
|	redirection_list redirection
;
simple_command:	simple_command_element
|	simple_command simple_command_element
;
command:	simple_command
|	T_LPARANTHESE compound_list T_RPARANTHESE
|	T_LPARANTHESE compound_list T_RPARANTHESE redirection_list
;
compound_list:	newline_list list
|	newline_list list T_NL newline_list
;
list:	list T_AND_IF newline_list list
|	list T_OR_IF newline_list list
|	list T_NL newline_list list
|	pipeline
;
newline_list:
|	newline_list T_NL
;
pipeline:	pipeline T_PIPE newline_list pipeline
|	command
;
%%

#include "your_header.h" // Assuming all structs, makers, and token definitions are here

/*
** ========================================================================== **
**                      SIMPLE COMMAND AND REDIRECTION PARSING
** ========================================================================== **
*/

t_element	*parse_simple_command_element(void)
{
	t_element	*elem;

	elem = ft_calloc(sizeof(t_element), 1);
	if (get_current_token()->token_type == T_WORD)
	{
		elem->word = make_word(get_current_token()->value,
				get_current_token()->flags);
		if (!elem->word)
			return (NULL);
		get_next_token();
	}
	else
	{
		elem->redirect = parse_redirection();
		if (!elem->redirect)
		{
			free(elem);
			return (NULL);
		}
	}
	return (elem);
}

t_redirect	*parse_redirection(void)
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;

	source = NULL;
	if (get_current_token()->token_type == T_NUMBER_LESS
		|| get_current_token()->token_type == T_NUMBER_GREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		source = make_word(ft_itoa(ft_atoi(get_current_token()->value)), 0);
	if (get_current_token()->token_type == T_GREAT
		|| get_current_token()->token_type == T_NUMBER_GREAT)
		redir_type = REDIR_OUTPUT;
	else if (get_current_token()->token_type == T_LESS
		|| get_current_token()->token_type == T_NUMBER_LESS)
		redir_type = REDIR_INPUT;
	else if (get_current_token()->token_type == T_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT)
		redir_type = REDIR_APPEND;
	else if (get_current_token()->token_type == T_DLESS
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		redir_type = REDIR_UNTIL;
	else
		return (NULL);
	get_next_token();
	if (!source)
	{
		if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
			source = make_word("1", 0);
		else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
			source = make_word("0", 0);
	}
	if (get_current_token()->token_type == T_WORD)
	{
		dest = make_word(get_current_token()->value,
				get_current_token()->flags);
		if (!dest)
			return (NULL); // leaks source word if it was allocated
		get_next_token();
	}
	else
		return (ft_panic());
	redirect = make_redirection(source, redir_type, dest);
	if (!redirect)
		return (NULL);
	if (redir_type == REDIR_UNTIL)
		make_here_document(redirect);
	return (redirect);
}

t_redirect	*parse_redirection_list(void)
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection();
	if (!current)
		return (NULL);
	head = current;
	while (1)
	{
		// Peek at the next token to see if it's another redirection
		if (get_current_token()->token_type != T_GREAT
			&& get_current_token()->token_type != T_LESS
			&& get_current_token()->token_type != T_DGREAT
			&& get_current_token()->token_type != T_DLESS
			&& get_current_token()->token_type != T_NUMBER_GREAT
			&& get_current_token()->token_type != T_NUMBER_LESS
			&& get_current_token()->token_type != T_NUMBER_DGREAT
			&& get_current_token()->token_type != T_NUMBER_DLESS)
			break ;
		current = parse_redirection();
		if (!current)
			return (ft_panic()); // Syntax error in subsequent redirection
		current->next = head;
		head = current;
	}
	return (head);
}

/*
** ========================================================================== **
**                      COMMAND AND LIST PARSING HIERARCHY
** ========================================================================== **
*/

t_command	*parse_simple_command(void)
{
	t_command		*cmd;
	int				element_parsed;
	t_token_type	token_type;
	t_element		*element;

	cmd = make_bare_simple_command();
	if (!cmd)
		return (NULL);
	element_parsed = 0;
	while (1)
	{
		token_type = get_current_token()->token_type;
		if (token_type == T_WORD || token_type == T_GREAT
			|| token_type == T_LESS || token_type == T_DGREAT
			|| token_type == T_DLESS || token_type == T_NUMBER_DGREAT
			|| token_type == T_NUMBER_DLESS || token_type == T_NUMBER_LESS
			|| token_type == T_NUMBER_GREAT)
		{
			element = parse_simple_command_element();
			if (!element)
				return (ft_panic());
			element_parsed = 1;
			cmd = make_simple_command(element, cmd);
			if (!cmd)
				return (NULL);
		}
		else
			break ;
	}
	if (!element_parsed)
		return (ft_panic());
	return (clean_simple_command(cmd));
}

t_command	*parse_command(void)
{
	t_command	*cmd;
	t_command	*sub_cmd;
	t_redirect	*redirects;

	if (get_current_token()->token_type == T_LPARANTHESE)
	{
		get_next_token();
		sub_cmd = parse_compound_list();
		if (!sub_cmd)
			return (ft_panic());
		cmd = make_subshell_command(sub_cmd);
		if (!cmd)
			return (ft_panic());
		if (!consume_token(T_RPARANTHESE)) // Assuming consume_token exists
			return (ft_panic());
		redirects = parse_redirection_list();
		if (redirects)
			cmd->redirects = ft_revredir(redirects);
	}
	else
	{
		cmd = parse_simple_command();
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

t_command	*parse_pipeline(void)
{
	t_command	*left;
	t_command	*right;

	left = parse_command();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_PIPE)
	{
		get_next_token();
		parse_newline_list();
		right = parse_command();
		if (!right)
			return (ft_panic());
		left = command_connect(left, right, CNT_PIPE);
	}
	return (left);
}

t_command	*parse_list(void)
{
	t_command		*left;
	t_command		*right;
	t_cnt_type		cnt_type;

	left = parse_pipeline();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_AND_IF
		|| get_current_token()->token_type == T_OR_IF
		|| get_current_token()->token_type == T_NL)
	{
		if (get_current_token()->token_type == T_AND_IF)
			cnt_type = CNT_AND_AND;
		else if (get_current_token()->token_type == T_OR_IF)
			cnt_type = CNT_OR_OR;
		else
			cnt_type = CNT_NL;
		get_next_token();
		parse_newline_list();
		right = parse_pipeline();
		if (!right)
			return (ft_panic());
		left = command_connect(left, right, cnt_type);
	}
	return (left);
}

void	parse_newline_list(void)
{
	while (get_current_token()->token_type == T_NL)
		get_next_token();
}

t_command	*parse_compound_list(void)
{
	t_command	*list_cmd;

	parse_newline_list();
	list_cmd = parse_list();
	if (!list_cmd)
		return (NULL);
	parse_newline_list(); // Handles optional trailing newlines before ')'
	return (list_cmd);
}

/*
** ========================================================================== **
**                          TOP-LEVEL PARSER ENTRYPOINT
** ========================================================================== **
*/

/**
 * @brief Parses a complete line of input based on the new grammar.
 *
 * Implements the grammar rules:
 *   inputunit: list T_NL
 *            | T_NL
 *            | error T_NL
 * This function is the entry point for parsing user input. It handles
 * empty lines and ensures that a valid command is always terminated by a
 * newline, as T_EOF is no longer a valid terminator.
 *
 * @return A pointer to the root of the command AST, or NULL if the line
 *         was empty. On syntax error, it calls ft_panic() and may return NULL.
 */
t_command	*parse_inputunit(void)
{
	t_command	*cmd;

	parse_newline_list();
	cmd = parse_list();
	if (!cmd)
		return (ft_panic());
	if (get_current_token()->token_type == T_NL)
		return (cmd);
	return (ft_panic());
}
