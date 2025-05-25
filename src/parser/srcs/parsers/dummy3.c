t_command	*parse_inputunit(void)
{
	t_command	*cmd;

	cmd = NULL;
	// inputunit: simple_list list_terminator
	//          | T_NEWLINE
	//          | error T_NEWLINE (error recovery - advanced)
	//          | error T_EOF     (error recovery - advanced)
	//          | T_EOF

	if (get_current_token()->token_type == T_EOF)
		return (NULL); // Empty input
	if (get_current_token()->token_type == T_NEWLINE)
	{
		get_next_token(); // Consume T_NEWLINE
		// An empty line is a valid input, might result in no command to execute.
		// Depending on your shell's behavior, you might return a special
		// "no-op" command or NULL. Let's return NULL for no executable command.
		return (NULL);
	}

	cmd = parse_simple_list();
	if (cmd) // Successfully parsed a simple_list
	{
		if (!parse_list_terminator())
		{
			// Expected T_NEWLINE or T_EOF after simple_list
			// parser_error("Syntax error: Expected newline or EOF after command list");
			// free_command(cmd); // Important: cleanup partially parsed command
			return (NULL);
		}
	}
	else
	{
		// parse_simple_list returned NULL. This means either:
		// 1. The input was empty and parse_simple_list correctly returned NULL (e.g. only EOF/NEWLINE from start)
		// 2. A syntax error occurred within parse_simple_list or its children.
		// If we are not at EOF or NEWLINE here, it's likely an error.
		if (get_current_token()->token_type != T_EOF && get_current_token()->token_type != T_NEWLINE)
		{
			// parser_error("Syntax error in inputunit");
			return (NULL);
		}
		// If it was just EOF or NEWLINE from the start, returning NULL is correct.
	}
	return (cmd);
}

// --- List Terminator ---
static int parse_list_terminator(void)
{
	// list_terminator: T_NEWLINE | T_EOF
	if (get_current_token()->token_type == T_NEWLINE)
	{
		get_next_token(); // Consume T_NEWLINE
		return (1);
	}
	if (get_current_token()->token_type == T_EOF)
	{
		// Do not consume T_EOF, it's a marker for the end.
		return (1);
	}
	return (0); // Not a valid list terminator
}

// --- Redirection Parsing ---
static t_redirect	*parse_redirection(void)
{
	t_redirect		*redirect_node;
	t_word			*source_word; // Represents the FD (e.g., "1", "0")
	t_word			*dest_word;   // Represents the filename or heredoc delimiter
	t_redir_type	r_type;
	t_token_type	current_type;
	char			*fd_str_val;

	source_word = NULL;
	dest_word = NULL;
	current_type = get_current_token()->token_type;
	fd_str_val = NULL; // To hold the string for the FD

	// Extract FD string if it's a T_NUMBER_... token
	// Assuming get_current_token()->value is the full lexeme like "1>" or "2>>"
	// and ft_atoi can parse the leading number.
	if (current_type == T_NUMBER_GREATER || current_type == T_NUMBER_LESS ||
		current_type == T_NUMBER_GREATER_GREATER || current_type == T_NUMBER_LESS_LESS)
	{
		// We need to create a t_word for the FD.
		// ft_itoa(ft_atoi(...)) is a bit roundabout if value is "1>"
		// A helper to extract just the number string would be better.
		// For now, assuming ft_atoi correctly gets the number from "1>" etc.
		int fd_int = ft_atoi(get_current_token()->value); // Parses "1" from "1>"
		fd_str_val = ft_itoa(fd_int); // Converts 1 back to "1"
		if (!fd_str_val) /* Malloc error in ft_itoa */ return (NULL);
		source_word = make_word(fd_str_val, 0); // fd_str_val will be copied by make_word or needs to be managed
		// If make_word strdups, then free(fd_str_val) here. If not, source_word owns it.
		// Let's assume make_word strdups.
		if (!source_word) { free(fd_str_val); return (NULL); }
        // free(fd_str_val); // If make_word dups the string
	}


	// Determine redirection type based on current token
	if (current_type == T_GREAT || current_type == T_NUMBER_GREATER)
		r_type = REDIR_OUTPUT;
	else if (current_type == T_LESS || current_type == T_NUMBER_LESS)
		r_type = REDIR_INPUT;
	else if (current_type == T_GREATER_GREATER || current_type == T_NUMBER_GREATER_GREATER)
		r_type = REDIR_APPEND;
	else if (current_type == T_LESS_LESS || current_type == T_NUMBER_LESS_LESS)
		r_type = REDIR_UNTIL;
	else
	{
		// Not a redirection starting token.
		// If source_word was allocated from a T_NUMBER_ token, it needs to be freed.
		if (source_word) free_word(source_word); // free_word should free its char*
		return (NULL);
	}
	get_next_token(); // Consume the redirection operator token

	// If source_word (FD) was not set by a T_NUMBER_ token, create default.
	if (!source_word)
	{
		if (r_type == REDIR_OUTPUT || r_type == REDIR_APPEND)
			fd_str_val = ft_strdup("1");
		else // REDIR_INPUT || REDIR_UNTIL
			fd_str_val = ft_strdup("0");
		if (!fd_str_val) return (NULL);
		source_word = make_word(fd_str_val, 0);
        if (!source_word) { free(fd_str_val); return (NULL); }
        // free(fd_str_val); // If make_word dups
	}

	// Expect T_WORD for the destination filename or delimiter
	if (get_current_token()->token_type == T_WORD)
	{
		dest_word = make_word(get_current_token()->value, get_current_token()->flags);
		if (!dest_word)
		{
			free_word(source_word); // Cleanup
			return (NULL);
		}
		get_next_token(); // Consume T_WORD
	}
	else
	{
		// parser_error("Syntax error: Expected filename/delimiter after redirection operator");
		free_word(source_word); // Cleanup
		return (NULL);
	}

	redirect_node = make_redirection(source_word, r_type, dest_word);
	if (!redirect_node)
	{
		// make_redirection should handle freeing source_word and dest_word if it fails
		// or they should be freed here if make_redirection doesn't take ownership on failure.
		// Assuming make_redirection handles them or takes ownership.
		return (NULL);
	}

	if (r_type == REDIR_UNTIL)
	{
		// Your original code called make_here_document.
		// This implies make_redirection might not fill here_doc_eof.
		// Or that make_here_document does the reading.
		// Assuming make_redirection sets redirect_node->here_doc_eof from dest_word->word
		// and make_here_document uses that.
		make_here_document(redirect_node);
	}
	return (redirect_node);
}

// --- Redirection List ---
static t_redirect	*parse_redirection_list(void)
{
	t_redirect	*head;
	t_redirect	*current_redir_node;
	t_redirect	*new_redir_node;

	head = NULL;
	current_redir_node = NULL;

	// Parse the first redirection
	new_redir_node = parse_redirection();
	if (!new_redir_node)
		return (NULL); // No redirections found, this is not an error for the list itself.

	head = new_redir_node;
	current_redir_node = head;

	// Parse subsequent redirections and append to the list
	while (1)
	{
		// Peek to see if another redirection follows.
		// parse_redirection() returns NULL if current token doesn't start a redirection.
		t_token_type next_tok_type = get_current_token()->token_type;
		if (next_tok_type == T_GREAT || next_tok_type == T_LESS ||
			next_tok_type == T_NUMBER_GREATER || next_tok_type == T_NUMBER_LESS ||
			next_tok_type == T_GREATER_GREATER || next_tok_type == T_NUMBER_GREATER_GREATER ||
			next_tok_type == T_LESS_LESS || next_tok_type == T_NUMBER_LESS_LESS)
		{
			new_redir_node = parse_redirection();
			if (new_redir_node)
			{
				current_redir_node->next = new_redir_node;
				current_redir_node = new_redir_node;
			}
			else
			{
				// This would mean parse_redirection failed mid-list, which is an error.
				// It should have reported it.
				// free_redirect_list(head); // Cleanup
				return (NULL); // Propagate error
			}
		}
		else
		{
			break; // No more redirection tokens
		}
	}
	// If you were using ft_revredir because of how make_simple_command added them,
	// and you build the list directly here in forward order, ft_revredir might not be needed
	// for redirections attached to subshells (command->redirects).
	// However, simple_command redirections might still need it if make_simple_command appends to head.
	return (head);
}


// --- Simple Command Element ---
static t_element	*parse_simple_command_element(void)
{
	t_element		*elem;
	t_token_type	tok_type;

	elem = (t_element *)ft_calloc(1, sizeof(t_element));
	if (!elem)
		return (NULL); // Malloc error

	tok_type = get_current_token()->token_type;

	if (tok_type == T_WORD)
	{
		elem->word = make_word(get_current_token()->value, get_current_token()->flags);
		if (!elem->word)
		{
			free(elem);
			return (NULL); // Error in make_word
		}
		get_next_token(); // Consume T_WORD
	}
	else if (tok_type == T_GREAT || tok_type == T_LESS ||
			 tok_type == T_NUMBER_GREATER || tok_type == T_NUMBER_LESS ||
			 tok_type == T_GREATER_GREATER || tok_type == T_NUMBER_GREATER_GREATER ||
			 tok_type == T_LESS_LESS || tok_type == T_NUMBER_LESS_LESS)
	{
		elem->redirect = parse_redirection();
		if (!elem->redirect)
		{
			// parse_redirection failed, it should report its own error.
			free(elem);
			return (NULL);
		}
	}
	else
	{
		// Not a T_WORD and not a redirection starting token.
		// This element cannot be parsed.
		// parser_error("Syntax error: Expected word or redirection in simple command element");
		free(elem);
		return (NULL);
	}
	return (elem);
}

// --- Simple Command ---
static t_command	*parse_simple_command(void)
{
	t_command		*cmd_simple_node;
	t_element		*elem_node;
	int				element_parsed_flag;
	t_token_type	current_tok_type;

	cmd_simple_node = make_bare_simple_command();
	if (!cmd_simple_node)
		return (NULL); // Malloc error in make_bare_simple_command

	element_parsed_flag = 0;
	while (1)
	{
		current_tok_type = get_current_token()->token_type;
		// Check if current token can start a simple_command_element
		if (current_tok_type == T_WORD ||
			current_tok_type == T_GREAT || current_tok_type == T_LESS ||
			current_tok_type == T_NUMBER_GREATER || current_tok_type == T_NUMBER_LESS ||
			current_tok_type == T_GREATER_GREATER || current_tok_type == T_NUMBER_GREATER_GREATER ||
			current_tok_type == T_LESS_LESS || current_tok_type == T_NUMBER_LESS_LESS)
		{
			elem_node = parse_simple_command_element();
			if (!elem_node)
			{
				// Error in parsing element, should have been reported.
				// free_command(cmd_simple_node); // Cleanup
				return (NULL);
			}
			element_parsed_flag = 1;
			// make_simple_command appends the element's content to cmd_simple_node
			// It should handle freeing elem_node itself or its contents if not needed.
			if (!make_simple_command(elem_node, cmd_simple_node)) // Assuming make_simple_command returns NULL on error
			{
				// free_command(cmd_simple_node); // Cleanup
				// free_element(elem_node); // if make_simple_command didn't consume/free it
				return (NULL);
			}
		}
		else
		{
			break; // Token does not start a simple_command_element
		}
	}

	if (!element_parsed_flag)
	{
		// This means the first token check failed, so it wasn't a simple command.
		// This is not an error for parse_simple_command itself, as it might be
		// called speculatively. The caller (parse_command) handles it.
		free_command(cmd_simple_node); // Free the bare command
		return (NULL);
	}

	// cmd_simple_node = clean_simple_command(cmd_simple_node); // Your original call
	// Ensure clean_simple_command handles NULL or returns NULL on error.
	return (cmd_simple_node);
}


// --- Command ---
static t_command	*parse_command(void)
{
	t_command	*cmd_node;
	t_command	*sub_cmd_list_node; // Renamed for clarity (was sub_cmd)
	t_redirect	*redir_list_node;

	cmd_node = NULL;
	if (get_current_token()->token_type == T_LPARANTHESE)
	{
		get_next_token(); // Consume T_LPARANTHESE
		sub_cmd_list_node = parse_compound_list(); // Changed from parse_list to parse_compound_list
		if (!sub_cmd_list_node)
		{
			// parser_error("Syntax error: Expected compound_list after '('");
			return (NULL);
		}

		cmd_node = make_subshell_command(sub_cmd_list_node);
		if (!cmd_node)
		{
			// free_command(sub_cmd_list_node); // if make_subshell_command doesn't take ownership on fail
			return (NULL);
		}

		if (!consume_token(T_RPARANTHESE))
		{
			// parser_error("Syntax error: Expected ')' after compound_list");
			// free_command(cmd_node); // cmd_node contains sub_cmd_list_node
			return (NULL);
		}

		// Optional redirection_list for the subshell command
		redir_list_node = parse_redirection_list();
		if (redir_list_node)
		{
			// If your redirection lists are built reversed and need ft_revredir
			// cmd_node->redirects = ft_revredir(redir_list_node);
			// If parse_redirection_list now builds them in order:
			cmd_node->redirects = redir_list_node;
		}
	}
	else
	{
		// Try to parse a simple_command
		cmd_node = parse_simple_command();
		if (!cmd_node)
		{
			// Not a simple command, and not a subshell.
			// This could be a syntax error if a command was expected.
			// parse_simple_command returns NULL if it's not a simple_command, which is fine.
			// The caller (e.g. parse_pipeline) will determine if this is an error.
			return (NULL);
		}
	}
	return (cmd_node);
}

// --- Newline List ---
static void	parse_newline_list(void)
{
	// newline_list: /* empty */
	//             | newline_list T_NEWLINE
	while (get_current_token()->token_type == T_NEWLINE)
	{
		get_next_token(); // Consume T_NEWLINE
	}
}

// --- Compound List ---
static t_command	*parse_compound_list(void)
{
	t_command	*list_cmd;

	// compound_list: newline_list list
	//              | newline_list list T_NEWLINE newline_list

	parse_newline_list();
	list_cmd = parse_list();
	if (!list_cmd)
	{
		// parser_error("Syntax error: Expected list in compound_list");
		return (NULL);
	}

	// Check for optional T_NEWLINE newline_list at the end
	if (get_current_token()->token_type == T_NEWLINE)
	{
		get_next_token(); // Consume the T_NEWLINE
		parse_newline_list(); // Consume any further newlines
	}
	return (list_cmd);
}

// --- List (Handles AND_IF, OR_IF, NEWLINE connectors) ---
static t_command	*parse_list(void)
{
	t_command		*left_cmd_node;
	t_command		*right_cmd_node;
	t_cnt_type		connector_type;
	t_token_type	current_tok_type;

	// list: pipeline ( (T_AND_IF | T_OR_IF | T_NEWLINE) newline_list pipeline )*
	// (Rewritten for RDP)

	left_cmd_node = parse_pipeline();
	if (!left_cmd_node)
		return (NULL); // Error in pipeline parsing

	while (1)
	{
		current_tok_type = get_current_token()->token_type;
		if (current_tok_type == T_AND_IF || current_tok_type == T_OR_IF || current_tok_type == T_NEWLINE)
		{
			if (current_tok_type == T_AND_IF)
				connector_type = CNT_AND_AND;
			else if (current_tok_type == T_OR_IF)
				connector_type = CNT_OR_OR;
			else // T_NEWLINE
			{
				// T_NEWLINE acts as a sequential connector here.
				// Your t_cnt_type needs a type for this (e.g., CNT_SEQ or CNT_SEMICOLON if that's the intent)
				// For now, let's map it to CNT_AND_AND as a placeholder if you don't have CNT_SEQ.
				// This needs to be decided based on your shell's logic for newline as a list separator.
				// If newline is like ';', then a CNT_SEQ is appropriate.
				// If t_cnt_type only has PIPE, AND, OR, this is an issue.
				// Let's assume you add CNT_SEQ or similar.
				connector_type = CNT_SEQ; // <<<<<<<< IMPORTANT: Ensure CNT_SEQ or similar exists in t_cnt_type
				if (connector_type == 0 && CNT_AND_AND == 0) /* Check if CNT_SEQ is defined, if not maybe map to AND */
				    connector_type = CNT_AND_AND; // Fallback if CNT_SEQ not available
			}
			get_next_token(); // Consume the connector (T_AND_IF, T_OR_IF, T_NEWLINE)

			parse_newline_list(); // Consume any newlines after the connector

			right_cmd_node = parse_pipeline();
			if (!right_cmd_node)
			{
				// parser_error("Syntax error: Expected pipeline after connector in list");
				// free_command(left_cmd_node); // Cleanup
				return (NULL);
			}

			left_cmd_node = command_connect(left_cmd_node, right_cmd_node, connector_type);
			if (!left_cmd_node)
			{
				// Error in command_connect (e.g., malloc failure)
				// command_connect should ideally free its inputs if it fails and takes ownership
				// free_command(right_cmd_node); // If not handled by command_connect
				return (NULL);
			}
		}
		else
		{
			break; // No more list connectors
		}
	}
	return (left_cmd_node);
}

// --- Simple List (Handles AND_IF, OR_IF connectors only) ---
static t_command	*parse_simple_list(void)
{
	t_command		*left_cmd_node;
	t_command		*right_cmd_node;
	t_cnt_type		connector_type;
	t_token_type	current_tok_type;

	// simple_list: pipeline ( (T_AND_IF | T_OR_IF) pipeline )*
	// (Rewritten for RDP)

	left_cmd_node = parse_pipeline();
	if (!left_cmd_node)
		return (NULL); // Error in pipeline parsing

	while (1)
	{
		current_tok_type = get_current_token()->token_type;
		if (current_tok_type == T_AND_IF || current_tok_type == T_OR_IF)
		{
			if (current_tok_type == T_AND_IF)
				connector_type = CNT_AND_AND;
			else // T_OR_IF
				connector_type = CNT_OR_OR;

			get_next_token(); // Consume the connector

			// Note: No parse_newline_list() here as per the 'simple_list' grammar difference

			right_cmd_node = parse_pipeline();
			if (!right_cmd_node)
			{
				// parser_error("Syntax error: Expected pipeline after connector in simple_list");
				// free_command(left_cmd_node); // Cleanup
				return (NULL);
			}

			left_cmd_node = command_connect(left_cmd_node, right_cmd_node, connector_type);
			if (!left_cmd_node)
			{
				// free_command(right_cmd_node); // If not handled by command_connect
				return (NULL);
			}
		}
		else
		{
			break; // No more simple_list connectors
		}
	}
	return (left_cmd_node);
}

// --- Pipeline ---
static t_command	*parse_pipeline(void)
{
	t_command		*left_cmd_node;
	t_command		*right_cmd_node;

	// pipeline: command ( T_PIPE command )*
	// (Rewritten for RDP, results in left-associativity for the AST)

	left_cmd_node = parse_command();
	if (!left_cmd_node)
		return (NULL); // Error in command parsing

	while (get_current_token()->token_type == T_PIPE)
	{
		get_next_token(); // Consume T_PIPE

		// Note: No parse_newline_list() here as per the 'pipeline' grammar.
		// If newlines were allowed around pipes, the grammar or this function would need it.

		right_cmd_node = parse_command();
		if (!right_cmd_node)
		{
			// parser_error("Syntax error: Expected command after T_PIPE in pipeline");
			// free_command(left_cmd_node); // Cleanup
			return (NULL);
		}

		left_cmd_node = command_connect(left_cmd_node, right_cmd_node, CNT_PIPE);
		if (!left_cmd_node)
		{
			// free_command(right_cmd_node); // If not handled by command_connect
			return (NULL);
		}
	}
	return (left_cmd_node);
}
