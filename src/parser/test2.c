#include <stdlib.h>

typedef struct command COMMAND;

enum command_type
{
	cm_for,
	cm_case,
	cm_while,
	cm_if,
	cm_simple,
	cm_select,
	cm_connection,
	cm_function_def,
	cm_until,
	cm_group,
	cm_arith,
	cm_cond,
	cm_arith_for,
	cm_subshell,
	cm_coproc
};

typedef struct word_desc
{
	char *word;
	int flags;

} WORD_DESC;

typedef union
{
	int dest;
	WORD_DESC *filename;

} REDIRECTEE;

enum r_instruction
{
	r_output_direction,
	r_input_direction,
	r_inputa_direction,
	r_appending_to,
	r_reading_until,
	r_reading_string,
	r_duplicating_input,
	r_duplicating_output,
	r_deblank_reading_until,
	r_close_this,
	r_err_and_out,
	r_input_output,
	r_output_force,
	r_duplicating_input_word,
	r_duplicating_output_word,
	r_move_input,
	r_move_output,
	r_move_input_word,
	r_move_output_word,
	r_append_err_and_out
};

typedef struct redirect
{
	struct redirect *next;
	REDIRECTEE redirector;
	int rflags;
	int flags;
	enum r_instruction  instruction;
	REDIRECTEE redirectee;
	char *here_doc_eof;

} REDIRECT;

typedef struct word_list
{
	struct word_list *next;
	WORD_DESC *word;

} WORD_LIST;

typedef struct for_com
{
	int flags;
	int line;
	WORD_DESC *name;
	WORD_LIST *map_list;
	COMMAND *action;

} FOR_COM;

typedef struct pattern_list
{
	struct pattern_list *next;
	WORD_LIST *patterns;
	COMMAND *action;
	int flags;

} PATTERN_LIST;

typedef struct case_com
{
	int flags;
	int line;
	WORD_DESC *word;
	PATTERN_LIST *clauses;

} CASE_COM;

typedef struct while_com
{
	int flags;
	COMMAND *test;
	COMMAND *action;

} WHILE_COM;

typedef struct if_com
{
	int flags;
	COMMAND *test;
	COMMAND *true_case;
	COMMAND *false_case;

} IF_COM;

typedef struct command {
	enum command_type type;
	int flags;
	int line;
	REDIRECT *redirects;
	union {
	  struct for_com *For;
	  struct case_com *Case;
	  struct while_com *While;
	  struct if_com *If;
	  struct connection *Connection;
	  struct simple_com *Simple;
	  struct function_def *Function_def;
	  struct group_com *Group;
  #if defined (SELECT_COMMAND)
	  struct select_com *Select;
  #endif
  #if defined (DPAREN_ARITHMETIC)
	  struct arith_com *Arith;
  #endif
  #if defined (COND_COMMAND)
	  struct cond_com *Cond;
  #endif
  #if defined (ARITH_FOR_COMMAND)
	  struct arith_for_com *ArithFor;
  #endif
	  struct subshell_com *Subshell;
	  struct coproc_com *Coproc;
	} value;
  } COMMAND;

COMMAND *global_command = (COMMAND *)NULL;

void	ft_test(int yyn)
{
	switch (yyn)
	{
	case 2:

		global_command = (yyvsp[-1].command);
		eof_encountered = 0;

		if (parser_state & PST_CMDSUBST)
			parser_state |= PST_EOFTOKEN;
		YYACCEPT;
	break;

	case 3:

		global_command = (COMMAND *)NULL;
		if (parser_state & PST_CMDSUBST)
			parser_state |= PST_EOFTOKEN;
		YYACCEPT;
	break;

	case 4:

		global_command = (COMMAND *)NULL;
		eof_encountered = 0;

		if (interactive && parse_and_execute_level == 0)
		{
			YYACCEPT;
		}
		else
		{
			YYABORT;
		}
	break;

	case 5:

		global_command = (COMMAND *)NULL;
		if (last_command_exit_value == 0)
			last_command_exit_value = EX_BADUSAGE;
		if (interactive && parse_and_execute_level == 0)
		{
			handle_eof_input_unit();
			YYACCEPT;
		}
		else
		{
			YYABORT;
		}
	break;

	case 6:

		global_command = (COMMAND *)NULL;
		handle_eof_input_unit();
		YYACCEPT;
	break;

	case 7:
		(yyval.word_list) = make_word_list((yyvsp[0].word), (WORD_LIST *)NULL);
	break;

	case 8:
		(yyval.word_list) = make_word_list((yyvsp[0].word), (yyvsp[-1].word_list));
	break;

	case 9:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_direction, redir, 0);
	break;

	case 10:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_direction, redir, 0);
	break;

	case 11:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_direction, redir, 0);
	break;

	case 12:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_direction, redir, 0);
	break;

	case 13:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_direction, redir, REDIR_VARASSIGN);
	break;

	case 14:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_direction, redir, REDIR_VARASSIGN);
	break;

	case 15:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_appending_to, redir, 0);
	break;

	case 16:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_appending_to, redir, 0);
	break;

	case 17:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_appending_to, redir, REDIR_VARASSIGN);
	break;

	case 18:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_force, redir, 0);
	break;

	case 19:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_force, redir, 0);
	break;

	case 20:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_output_force, redir, REDIR_VARASSIGN);
	break;

	case 21:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_output, redir, 0);
	break;

	case 22:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_output, redir, 0);
	break;

	case 23:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_input_output, redir, REDIR_VARASSIGN);
	break;

	case 24:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_until, redir, 0);
		push_heredoc((yyval.redirect));
	break;

	case 25:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_until, redir, 0);
		push_heredoc((yyval.redirect));
	break;

	case 26:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_until, redir, REDIR_VARASSIGN);
		push_heredoc((yyval.redirect));
	break;

	case 27:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_deblank_reading_until, redir, 0);
		push_heredoc((yyval.redirect));
	break;

	case 28:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_deblank_reading_until, redir, 0);
		push_heredoc((yyval.redirect));
	break;

	case 29:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_deblank_reading_until, redir, REDIR_VARASSIGN);
		push_heredoc((yyval.redirect));
	break;

	case 30:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_string, redir, 0);
	break;

	case 31:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_string, redir, 0);
	break;

	case 32:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_reading_string, redir, REDIR_VARASSIGN);
	break;

	case 33:
		source.dest = 0;
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_input, redir, 0);
	break;

	case 34:
		source.dest = (yyvsp[-2].number);
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_input, redir, 0);
	break;

	case 35:
		source.filename = (yyvsp[-2].word);
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_input, redir, REDIR_VARASSIGN);
	break;

	case 36:
		source.dest = 1;
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_output, redir, 0);
	break;

	case 37:
		source.dest = (yyvsp[-2].number);
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_output, redir, 0);
	break;

	case 38:
		source.filename = (yyvsp[-2].word);
		redir.dest = (yyvsp[0].number);
		(yyval.redirect) = make_redirection(source, r_duplicating_output, redir, REDIR_VARASSIGN);
	break;

	case 39:
		source.dest = 0;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_input_word, redir, 0);
	break;

	case 40:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_input_word, redir, 0);
	break;

	case 41:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_input_word, redir, REDIR_VARASSIGN);
	break;

	case 42:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_output_word, redir, 0);
	break;

	case 43:
		source.dest = (yyvsp[-2].number);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_output_word, redir, 0);
	break;

	case 44:
		source.filename = (yyvsp[-2].word);
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_duplicating_output_word, redir, REDIR_VARASSIGN);
	break;

	case 45:
		source.dest = 1;
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, 0);
	break;

	case 46:
		source.dest = (yyvsp[-2].number);
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, 0);
	break;

	case 47:
		source.filename = (yyvsp[-2].word);
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, REDIR_VARASSIGN);
	break;

	case 48:
		source.dest = 0;
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, 0);
	break;

	case 49:
		source.dest = (yyvsp[-2].number);
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, 0);
	break;

	case 50:
		source.filename = (yyvsp[-2].word);
		redir.dest = 0;
		(yyval.redirect) = make_redirection(source, r_close_this, redir, REDIR_VARASSIGN);
	break;

	case 51:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_err_and_out, redir, 0);
	break;

	case 52:
		source.dest = 1;
		redir.filename = (yyvsp[0].word);
		(yyval.redirect) = make_redirection(source, r_append_err_and_out, redir, 0);
	break;

	case 53:
		(yyval.element).word = (yyvsp[0].word);
		(yyval.element).redirect = 0;
	break;

	case 54:
		(yyval.element).word = (yyvsp[0].word);
		(yyval.element).redirect = 0;
	break;

	case 55:
		(yyval.element).redirect = (yyvsp[0].redirect);
		(yyval.element).word = 0;
	break;

	case 56:
		(yyval.redirect) = (yyvsp[0].redirect);
	break;

	case 57:
		register REDIRECT *t;

		for (t = (yyvsp[-1].redirect); t->next; t = t->next)
			;
		t->next = (yyvsp[0].redirect);
		(yyval.redirect) = (yyvsp[-1].redirect);
	break;

	case 58:
		(yyval.command) = make_simple_command((yyvsp[0].element), (COMMAND *)NULL);
	break;

	case 59:
		(yyval.command) = make_simple_command((yyvsp[0].element), (yyvsp[-1].command));
	break;

	case 60:
		(yyval.command) = clean_simple_command((yyvsp[0].command));
	break;

	case 61:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 62:
		COMMAND *tc;

		tc = (yyvsp[-1].command);
		if (tc && tc->redirects)
		{
			register REDIRECT *t;
			for (t = tc->redirects; t->next; t = t->next)
				;
			t->next = (yyvsp[0].redirect);
		}
		else if (tc)
			tc->redirects = (yyvsp[0].redirect);
		(yyval.command) = (yyvsp[-1].command);
	break;

	case 63:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 64:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 65:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 66:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 67:
		(yyval.command) = make_while_command((yyvsp[-3].command), (yyvsp[-1].command));
	break;

	case 68:
		(yyval.command) = make_until_command((yyvsp[-3].command), (yyvsp[-1].command));
	break;

	case 69:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 70:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 71:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 72:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 73:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 74:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 75:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 76:
		(yyval.command) = make_for_command((yyvsp[-4].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 77:
		(yyval.command) = make_for_command((yyvsp[-4].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 78:
		(yyval.command) = make_for_command((yyvsp[-5].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 79:
		(yyval.command) = make_for_command((yyvsp[-5].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 80:
		(yyval.command) = make_for_command((yyvsp[-8].word), REVERSE_LIST((yyvsp[-5].word_list), WORD_LIST *), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 81:
		(yyval.command) = make_for_command((yyvsp[-8].word), REVERSE_LIST((yyvsp[-5].word_list), WORD_LIST *), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 82:
		(yyval.command) = make_for_command((yyvsp[-7].word), (WORD_LIST *)NULL, (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 83:
		(yyval.command) = make_for_command((yyvsp[-7].word), (WORD_LIST *)NULL, (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 84:
		(yyval.command) = make_arith_for_command((yyvsp[-5].word_list), (yyvsp[-1].command), arith_for_lineno);
		if ((yyval.command) == 0)
			YYERROR;
		if (word_top > 0)
			word_top--;
	break;

	case 85:
		(yyval.command) = make_arith_for_command((yyvsp[-5].word_list), (yyvsp[-1].command), arith_for_lineno);
		if ((yyval.command) == 0)
			YYERROR;
		if (word_top > 0)
			word_top--;
	break;

	case 86:
		(yyval.command) = make_arith_for_command((yyvsp[-3].word_list), (yyvsp[-1].command), arith_for_lineno);
		if ((yyval.command) == 0)
			YYERROR;
		if (word_top > 0)
			word_top--;
	break;

	case 87:
		(yyval.command) = make_arith_for_command((yyvsp[-3].word_list), (yyvsp[-1].command), arith_for_lineno);
		if ((yyval.command) == 0)
			YYERROR;
		if (word_top > 0)
			word_top--;
	break;

	case 88:
		(yyval.command) = make_select_command((yyvsp[-4].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 89:
		(yyval.command) = make_select_command((yyvsp[-4].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 90:
		(yyval.command) = make_select_command((yyvsp[-5].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 91:
		(yyval.command) = make_select_command((yyvsp[-5].word), add_string_to_list("\"$@\"", (WORD_LIST *)NULL), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 92:
		(yyval.command) = make_select_command((yyvsp[-8].word), REVERSE_LIST((yyvsp[-5].word_list), WORD_LIST *), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 93:
		(yyval.command) = make_select_command((yyvsp[-8].word), REVERSE_LIST((yyvsp[-5].word_list), WORD_LIST *), (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 94:
		(yyval.command) = make_select_command((yyvsp[-7].word), (WORD_LIST *)NULL, (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 95:
		(yyval.command) = make_select_command((yyvsp[-7].word), (WORD_LIST *)NULL, (yyvsp[-1].command), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 96:
		(yyval.command) = make_case_command((yyvsp[-4].word), (PATTERN_LIST *)NULL, word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 97:
		(yyval.command) = make_case_command((yyvsp[-5].word), (yyvsp[-2].pattern), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 98:
		(yyval.command) = make_case_command((yyvsp[-4].word), (yyvsp[-1].pattern), word_lineno[word_top]);
		if (word_top > 0)
			word_top--;
	break;

	case 99:
		(yyval.command) = make_function_def((yyvsp[-4].word), (yyvsp[0].command), function_dstart, function_bstart);
	break;

	case 100:
		(yyval.command) = make_function_def((yyvsp[-4].word), (yyvsp[0].command), function_dstart, function_bstart);
	break;

	case 101:
		(yyval.command) = make_function_def((yyvsp[-1].word), (yyvsp[0].command), function_dstart, function_bstart);
	break;

	case 102:
		(yyval.command) = make_function_def((yyvsp[-3].word), (yyvsp[0].command), function_dstart, function_bstart);
	break;

	case 103:
		(yyval.command) = (yyvsp[0].command);
	break;

	case 104:
		COMMAND *tc;

		tc = (yyvsp[-1].command);

		if (tc && tc->redirects)
		{
			register REDIRECT *t;
			for (t = tc->redirects; t->next; t = t->next)
				;
			t->next = (yyvsp[0].redirect);
		}
		else if (tc)
			tc->redirects = (yyvsp[0].redirect);
		(yyval.command) = (yyvsp[-1].command);
	break;

	case 105:
		(yyval.command) = make_subshell_command((yyvsp[-1].command));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL;
	break;

	case 106:
		(yyval.command) = make_coproc_command("COPROC", (yyvsp[0].command));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL | CMD_COPROC_SUBSHELL;
	break;

	case 107:
		COMMAND *tc;

		tc = (yyvsp[-1].command);
		if (tc && tc->redirects)
		{
			register REDIRECT *t;
			for (t = tc->redirects; t->next; t = t->next)
				;
			t->next = (yyvsp[0].redirect);
		}
		else if (tc)
			tc->redirects = (yyvsp[0].redirect);
		(yyval.command) = make_coproc_command("COPROC", (yyvsp[-1].command));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL | CMD_COPROC_SUBSHELL;
	break;

	case 108:

		(yyval.command) = make_coproc_command((yyvsp[-1].word)->word, (yyvsp[0].command));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL | CMD_COPROC_SUBSHELL;
	break;

	case 109:

		COMMAND *tc;

		tc = (yyvsp[-1].command);
		if (tc && tc->redirects)
		{
			register REDIRECT *t;
			for (t = tc->redirects; t->next; t = t->next)
				;
			t->next = (yyvsp[0].redirect);
		}
		else if (tc)
			tc->redirects = (yyvsp[0].redirect);
		(yyval.command) = make_coproc_command((yyvsp[-2].word)->word, (yyvsp[-1].command));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL | CMD_COPROC_SUBSHELL;
	break;

	case 110:

		(yyval.command) = make_coproc_command("COPROC", clean_simple_command((yyvsp[0].command)));
		(yyval.command)->flags |= CMD_WANT_SUBSHELL | CMD_COPROC_SUBSHELL;
	break;

	case 111:

		(yyval.command) = make_if_command((yyvsp[-3].command), (yyvsp[-1].command), (COMMAND *)NULL);
	break;

	case 112:

		(yyval.command) = make_if_command((yyvsp[-5].command), (yyvsp[-3].command), (yyvsp[-1].command));
	break;

	case 113:

		(yyval.command) = make_if_command((yyvsp[-4].command), (yyvsp[-2].command), (yyvsp[-1].command));
	break;

	case 114:

		(yyval.command) = make_group_command((yyvsp[-1].command));
	break;

	case 115:

		(yyval.command) = make_arith_command((yyvsp[0].word_list));
	break;

	case 116:

		(yyval.command) = (yyvsp[-1].command);
	break;

	case 117:

		(yyval.command) = make_if_command((yyvsp[-2].command), (yyvsp[0].command), (COMMAND *)NULL);
	break;

	case 118:

		(yyval.command) = make_if_command((yyvsp[-4].command), (yyvsp[-2].command), (yyvsp[0].command));
	break;

	case 119:

		(yyval.command) = make_if_command((yyvsp[-3].command), (yyvsp[-1].command), (yyvsp[0].command));
	break;

	case 121:

		(yyvsp[0].pattern)->next = (yyvsp[-1].pattern);
		(yyval.pattern) = (yyvsp[0].pattern);
	break;

	case 122:

		(yyval.pattern) = make_pattern_list((yyvsp[-2].word_list), (yyvsp[0].command));
	break;

	case 123:

		(yyval.pattern) = make_pattern_list((yyvsp[-2].word_list), (COMMAND *)NULL);
	break;

	case 124:

		(yyval.pattern) = make_pattern_list((yyvsp[-2].word_list), (yyvsp[0].command));
	break;

	case 125:

		(yyval.pattern) = make_pattern_list((yyvsp[-2].word_list), (COMMAND *)NULL);
	break;

	case 126:

		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 127:

		(yyvsp[-1].pattern)->next = (yyvsp[-2].pattern);
		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 128:

		(yyvsp[-1].pattern)->flags |= CASEPAT_FALLTHROUGH;
		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 129:

		(yyvsp[-1].pattern)->flags |= CASEPAT_FALLTHROUGH;
		(yyvsp[-1].pattern)->next = (yyvsp[-2].pattern);
		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 130:

		(yyvsp[-1].pattern)->flags |= CASEPAT_TESTNEXT;
		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 131:

		(yyvsp[-1].pattern)->flags |= CASEPAT_TESTNEXT;
		(yyvsp[-1].pattern)->next = (yyvsp[-2].pattern);
		(yyval.pattern) = (yyvsp[-1].pattern);
	break;

	case 132:

		(yyval.word_list) = make_word_list((yyvsp[0].word), (WORD_LIST *)NULL);
	break;

	case 133:

		(yyval.word_list) = make_word_list((yyvsp[0].word), (yyvsp[-2].word_list));
	break;

	case 134:

		(yyval.command) = (yyvsp[0].command);
		if (need_here_doc)
			gather_here_documents();
	break;

	case 136:

		(yyval.command) = (yyvsp[0].command);
	break;

	case 138:

		if ((yyvsp[-2].command)->type == cm_connection)
			(yyval.command) = connect_async_list((yyvsp[-2].command), (COMMAND *)NULL, '&');
		else
			(yyval.command) = command_connect((yyvsp[-2].command), (COMMAND *)NULL, '&');
	break;

	case 140:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), AND_AND);
	break;

	case 141:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), OR_OR);
	break;

	case 142:

		if ((yyvsp[-3].command)->type == cm_connection)
			(yyval.command) = connect_async_list((yyvsp[-3].command), (yyvsp[0].command), '&');
		else
			(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), '&');
	break;

	case 143:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), ';');
	break;

	case 144:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), ';');
	break;

	case 145:

		(yyval.command) = (yyvsp[0].command);
	break;

	case 148:

		(yyval.number) = '\n';
	break;

	case 149:

		(yyval.number) = ';';
	break;

	case 150:

		(yyval.number) = yacc_EOF;
	break;

	case 153:

		(yyval.command) = (yyvsp[0].command);
		if (need_here_doc)
			gather_here_documents();
		if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
		{
			global_command = (yyvsp[0].command);
			eof_encountered = 0;
			rewind_input_string();
			YYACCEPT;
		}
	break;

	case 154:

		if ((yyvsp[-1].command)->type == cm_connection)
			(yyval.command) = connect_async_list((yyvsp[-1].command), (COMMAND *)NULL, '&');
		else
			(yyval.command) = command_connect((yyvsp[-1].command), (COMMAND *)NULL, '&');
		if (need_here_doc)
			gather_here_documents();
		if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
		{
			global_command = (yyvsp[-1].command);
			eof_encountered = 0;
			rewind_input_string();
			YYACCEPT;
		}
	break;

	case 155:

		(yyval.command) = (yyvsp[-1].command);
		if (need_here_doc)
			gather_here_documents();
		if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
		{
			global_command = (yyvsp[-1].command);
			eof_encountered = 0;
			rewind_input_string();
			YYACCEPT;
		}
	break;

	case 156:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), AND_AND);
	break;

	case 157:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), OR_OR);
	break;

	case 158:

		if ((yyvsp[-2].command)->type == cm_connection)
			(yyval.command) = connect_async_list((yyvsp[-2].command), (yyvsp[0].command), '&');
		else
			(yyval.command) = command_connect((yyvsp[-2].command), (yyvsp[0].command), '&');
	break;

	case 159:

		(yyval.command) = command_connect((yyvsp[-2].command), (yyvsp[0].command), ';');
	break;

	case 160:

		(yyval.command) = (yyvsp[0].command);
	break;

	case 161:

		(yyval.command) = (yyvsp[0].command);
	break;

	case 162:

		if ((yyvsp[0].command))
			(yyvsp[0].command)->flags ^= CMD_INVERT_RETURN;
		(yyval.command) = (yyvsp[0].command);
	break;

	case 163:

		if ((yyvsp[0].command))
			(yyvsp[0].command)->flags |= (yyvsp[-1].number);
		(yyval.command) = (yyvsp[0].command);
	break;

	case 164:

		ELEMENT x;

		x.word = 0;
		x.redirect = 0;
		(yyval.command) = make_simple_command(x, (COMMAND *)NULL);
		(yyval.command)->flags |= (yyvsp[-1].number);

		if ((yyvsp[0].number) == '\n')
			token_to_read = '\n';
		else if ((yyvsp[0].number) == ';')
			token_to_read = ';';
		parser_state &= ~PST_REDIRLIST;
	break;

	case 165:

		ELEMENT x;

		x.word = 0;
		x.redirect = 0;
		(yyval.command) = make_simple_command(x, (COMMAND *)NULL);
		(yyval.command)->flags |= CMD_INVERT_RETURN;

		if ((yyvsp[0].number) == '\n')
			token_to_read = '\n';
		if ((yyvsp[0].number) == ';')
			token_to_read = ';';
		parser_state &= ~PST_REDIRLIST;
	break;

	case 166:

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), '|');
	break;

	case 167:


		COMMAND *tc;
		REDIRECTEE rd, sd;
		REDIRECT *r;

		tc = (yyvsp[-3].command)->type == cm_simple ? (COMMAND *)(yyvsp[-3].command)->value.Simple : (yyvsp[-3].command);
		sd.dest = 2;
		rd.dest = 1;
		r = make_redirection(sd, r_duplicating_output, rd, 0);
		if (tc->redirects)
		{
			register REDIRECT *t;
			for (t = tc->redirects; t->next; t = t->next)
				;
			t->next = r;
		}
		else
			tc->redirects = r;

		(yyval.command) = command_connect((yyvsp[-3].command), (yyvsp[0].command), '|');
	break;

	case 168:

		(yyval.command) = (yyvsp[0].command);
	break;

	case 169:

		(yyval.number) = CMD_TIME_PIPELINE;
	break;

	case 170:

		(yyval.number) = CMD_TIME_PIPELINE | CMD_TIME_POSIX;
	break;

	case 171:

		(yyval.number) = CMD_TIME_PIPELINE | CMD_TIME_POSIX;
	break;

	case 172:

		(yyval.number) = CMD_TIME_PIPELINE | CMD_TIME_POSIX;
	break;

	default:
		break;

}
