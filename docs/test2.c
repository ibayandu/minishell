#include <stdlib.h>
#include <stdio.h>

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

typedef struct connection {
  int ignore;
  COMMAND *first;
  COMMAND *second;
  int connector;
} CONNECTION;

typedef struct simple_com {
  int flags;
  int line;
  WORD_LIST *words;
  REDIRECT *redirects;
} SIMPLE_COM;

typedef struct function_def {
  int flags;
  int line;
  WORD_DESC *name;
  COMMAND *command;
  char *source_file;
} FUNCTION_DEF;

typedef struct group_com {
  int ignore;
  COMMAND *command;
} GROUP_COM;

typedef struct subshell_com {
  int flags;
  int line;
  COMMAND *command;
} SUBSHELL_COM;

typedef struct coproc_com {
  int flags;
  char *name;
  COMMAND *command;
} COPROC_COM;

typedef struct element {
  WORD_DESC *word;
  REDIRECT *redirect;
} ELEMENT;

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

union YYSTYPE
{
  WORD_DESC *word;
  int number;
  WORD_LIST *word_list;
  COMMAND *command;
  REDIRECT *redirect;
  ELEMENT element;
  PATTERN_LIST *pattern;
};
typedef union YYSTYPE YYSTYPE;

COMMAND *global_command = (COMMAND *)NULL;

typedef int yy_state_fast_t;
typedef short yy_state_t;

enum yytokentype
{
	YYEMPTY = -2,
	YYEOF = 0,                     /* "end of file"  */
	YYerror = 256,                 /* error  */
	YYUNDEF = 257,                 /* "invalid token"  */
	IF = 258,                      /* IF  */
	THEN = 259,                    /* THEN  */
	ELSE = 260,                    /* ELSE  */
	ELIF = 261,                    /* ELIF  */
	FI = 262,                      /* FI  */
	CASE = 263,                    /* CASE  */
	ESAC = 264,                    /* ESAC  */
	FOR = 265,                     /* FOR  */
	SELECT = 266,                  /* SELECT  */
	WHILE = 267,                   /* WHILE  */
	UNTIL = 268,                   /* UNTIL  */
	DO = 269,                      /* DO  */
	DONE = 270,                    /* DONE  */
	FUNCTION = 271,                /* FUNCTION  */
	COPROC = 272,                  /* COPROC  */
	COND_START = 273,              /* COND_START  */
	COND_END = 274,                /* COND_END  */
	COND_ERROR = 275,              /* COND_ERROR  */
	IN = 276,                      /* IN  */
	BANG = 277,                    /* BANG  */
	TIME = 278,                    /* TIME  */
	TIMEOPT = 279,                 /* TIMEOPT  */
	TIMEIGN = 280,                 /* TIMEIGN  */
	WORD = 281,                    /* WORD  */
	ASSIGNMENT_WORD = 282,         /* ASSIGNMENT_WORD  */
	REDIR_WORD = 283,              /* REDIR_WORD  */
	NUMBER = 284,                  /* NUMBER  */
	ARITH_CMD = 285,               /* ARITH_CMD  */
	ARITH_FOR_EXPRS = 286,         /* ARITH_FOR_EXPRS  */
	COND_CMD = 287,                /* COND_CMD  */
	AND_AND = 288,                 /* AND_AND  */
	OR_OR = 289,                   /* OR_OR  */
	GREATER_GREATER = 290,         /* GREATER_GREATER  */
	LESS_LESS = 291,               /* LESS_LESS  */
	LESS_AND = 292,                /* LESS_AND  */
	LESS_LESS_LESS = 293,          /* LESS_LESS_LESS  */
	GREATER_AND = 294,             /* GREATER_AND  */
	SEMI_SEMI = 295,               /* SEMI_SEMI  */
	SEMI_AND = 296,                /* SEMI_AND  */
	SEMI_SEMI_AND = 297,           /* SEMI_SEMI_AND  */
	LESS_LESS_MINUS = 298,         /* LESS_LESS_MINUS  */
	AND_GREATER = 299,             /* AND_GREATER  */
	AND_GREATER_GREATER = 300,     /* AND_GREATER_GREATER  */
	LESS_GREATER = 301,            /* LESS_GREATER  */
	GREATER_BAR = 302,             /* GREATER_BAR  */
	BAR_AND = 303,                 /* BAR_AND  */
	yacc_EOF = 304                 /* yacc_EOF  */
};
typedef enum yytokentype yytoken_kind_t;

enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_THEN = 4,                       /* THEN  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_ELIF = 6,                       /* ELIF  */
  YYSYMBOL_FI = 7,                         /* FI  */
  YYSYMBOL_CASE = 8,                       /* CASE  */
  YYSYMBOL_ESAC = 9,                       /* ESAC  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_SELECT = 11,                    /* SELECT  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_UNTIL = 13,                     /* UNTIL  */
  YYSYMBOL_DO = 14,                        /* DO  */
  YYSYMBOL_DONE = 15,                      /* DONE  */
  YYSYMBOL_FUNCTION = 16,                  /* FUNCTION  */
  YYSYMBOL_COPROC = 17,                    /* COPROC  */
  YYSYMBOL_COND_START = 18,                /* COND_START  */
  YYSYMBOL_COND_END = 19,                  /* COND_END  */
  YYSYMBOL_COND_ERROR = 20,                /* COND_ERROR  */
  YYSYMBOL_IN = 21,                        /* IN  */
  YYSYMBOL_BANG = 22,                      /* BANG  */
  YYSYMBOL_TIME = 23,                      /* TIME  */
  YYSYMBOL_TIMEOPT = 24,                   /* TIMEOPT  */
  YYSYMBOL_TIMEIGN = 25,                   /* TIMEIGN  */
  YYSYMBOL_WORD = 26,                      /* WORD  */
  YYSYMBOL_ASSIGNMENT_WORD = 27,           /* ASSIGNMENT_WORD  */
  YYSYMBOL_REDIR_WORD = 28,                /* REDIR_WORD  */
  YYSYMBOL_NUMBER = 29,                    /* NUMBER  */
  YYSYMBOL_ARITH_CMD = 30,                 /* ARITH_CMD  */
  YYSYMBOL_ARITH_FOR_EXPRS = 31,           /* ARITH_FOR_EXPRS  */
  YYSYMBOL_COND_CMD = 32,                  /* COND_CMD  */
  YYSYMBOL_AND_AND = 33,                   /* AND_AND  */
  YYSYMBOL_OR_OR = 34,                     /* OR_OR  */
  YYSYMBOL_GREATER_GREATER = 35,           /* GREATER_GREATER  */
  YYSYMBOL_LESS_LESS = 36,                 /* LESS_LESS  */
  YYSYMBOL_LESS_AND = 37,                  /* LESS_AND  */
  YYSYMBOL_LESS_LESS_LESS = 38,            /* LESS_LESS_LESS  */
  YYSYMBOL_GREATER_AND = 39,               /* GREATER_AND  */
  YYSYMBOL_SEMI_SEMI = 40,                 /* SEMI_SEMI  */
  YYSYMBOL_SEMI_AND = 41,                  /* SEMI_AND  */
  YYSYMBOL_SEMI_SEMI_AND = 42,             /* SEMI_SEMI_AND  */
  YYSYMBOL_LESS_LESS_MINUS = 43,           /* LESS_LESS_MINUS  */
  YYSYMBOL_AND_GREATER = 44,               /* AND_GREATER  */
  YYSYMBOL_AND_GREATER_GREATER = 45,       /* AND_GREATER_GREATER  */
  YYSYMBOL_LESS_GREATER = 46,              /* LESS_GREATER  */
  YYSYMBOL_GREATER_BAR = 47,               /* GREATER_BAR  */
  YYSYMBOL_BAR_AND = 48,                   /* BAR_AND  */
  YYSYMBOL_49_ = 49,                       /* '&'  */
  YYSYMBOL_50_ = 50,                       /* ';'  */
  YYSYMBOL_51_n_ = 51,                     /* '\n'  */
  YYSYMBOL_yacc_EOF = 52,                  /* yacc_EOF  */
  YYSYMBOL_53_ = 53,                       /* '|'  */
  YYSYMBOL_54_ = 54,                       /* '>'  */
  YYSYMBOL_55_ = 55,                       /* '<'  */
  YYSYMBOL_56_ = 56,                       /* '-'  */
  YYSYMBOL_57_ = 57,                       /* '{'  */
  YYSYMBOL_58_ = 58,                       /* '}'  */
  YYSYMBOL_59_ = 59,                       /* '('  */
  YYSYMBOL_60_ = 60,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_inputunit = 62,                 /* inputunit  */
  YYSYMBOL_word_list = 63,                 /* word_list  */
  YYSYMBOL_redirection = 64,               /* redirection  */
  YYSYMBOL_simple_command_element = 65,    /* simple_command_element  */
  YYSYMBOL_redirection_list = 66,          /* redirection_list  */
  YYSYMBOL_simple_command = 67,            /* simple_command  */
  YYSYMBOL_command = 68,                   /* command  */
  YYSYMBOL_shell_command = 69,             /* shell_command  */
  YYSYMBOL_for_command = 70,               /* for_command  */
  YYSYMBOL_arith_for_command = 71,         /* arith_for_command  */
  YYSYMBOL_select_command = 72,            /* select_command  */
  YYSYMBOL_case_command = 73,              /* case_command  */
  YYSYMBOL_function_def = 74,              /* function_def  */
  YYSYMBOL_function_body = 75,             /* function_body  */
  YYSYMBOL_subshell = 76,                  /* subshell  */
  YYSYMBOL_coproc = 77,                    /* coproc  */
  YYSYMBOL_if_command = 78,                /* if_command  */
  YYSYMBOL_group_command = 79,             /* group_command  */
  YYSYMBOL_arith_command = 80,             /* arith_command  */
  YYSYMBOL_cond_command = 81,              /* cond_command  */
  YYSYMBOL_elif_clause = 82,               /* elif_clause  */
  YYSYMBOL_case_clause = 83,               /* case_clause  */
  YYSYMBOL_pattern_list = 84,              /* pattern_list  */
  YYSYMBOL_case_clause_sequence = 85,      /* case_clause_sequence  */
  YYSYMBOL_pattern = 86,                   /* pattern  */
  YYSYMBOL_list = 87,                      /* list  */
  YYSYMBOL_compound_list = 88,             /* compound_list  */
  YYSYMBOL_list0 = 89,                     /* list0  */
  YYSYMBOL_list1 = 90,                     /* list1  */
  YYSYMBOL_simple_list_terminator = 91,    /* simple_list_terminator  */
  YYSYMBOL_list_terminator = 92,           /* list_terminator  */
  YYSYMBOL_newline_list = 93,              /* newline_list  */
  YYSYMBOL_simple_list = 94,               /* simple_list  */
  YYSYMBOL_simple_list1 = 95,              /* simple_list1  */
  YYSYMBOL_pipeline_command = 96,          /* pipeline_command  */
  YYSYMBOL_pipeline = 97,                  /* pipeline  */
  YYSYMBOL_timespec = 98                   /* timespec  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;

static const short yypact[] =
{
     313,   108,  -204,    -6,     8,     2,  -204,  -204,    10,   513,
      17,   363,   153,   -21,  -204,   593,   606,  -204,    14,    26,
     113,    41,   127,    72,    85,    92,    95,    98,  -204,  -204,
     100,   105,  -204,  -204,    65,  -204,  -204,   551,  -204,   572,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,
    -204,   146,   140,  -204,    67,   363,  -204,  -204,  -204,   133,
     413,  -204,    93,    55,   104,   156,   161,    11,    45,   551,
     572,   163,  -204,  -204,  -204,  -204,  -204,   167,  -204,   152,
     208,   217,   129,   220,   150,   221,   223,   225,   233,   234,
     238,   239,   158,   240,   162,   241,   243,   244,   252,   253,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,
    -204,  -204,  -204,  -204,  -204,  -204,   194,   227,  -204,  -204,
    -204,  -204,   572,  -204,  -204,  -204,  -204,  -204,   463,   463,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,    -7,  -204,    59,
    -204,    52,  -204,  -204,  -204,  -204,    62,  -204,  -204,  -204,
     235,   572,  -204,   572,   572,  -204,  -204,  -204,  -204,  -204,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,
    -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,  -204,   413,
     413,   191,   191,   245,   245,   203,  -204,  -204,  -204,  -204,
    -204,  -204,    37,  -204,   176,  -204,   270,   228,    76,    79,
    -204,   176,  -204,   278,   282,   563,  -204,   572,   572,   563,
    -204,  -204,    67,    67,  -204,  -204,  -204,   291,   413,   413,
     413,   413,   413,   294,   175,  -204,    28,  -204,  -204,   292,
    -204,   187,  -204,   250,  -204,  -204,  -204,  -204,  -204,  -204,
     295,   413,   187,  -204,   251,  -204,  -204,  -204,   563,  -204,
     304,   314,  -204,  -204,  -204,   196,   196,   196,  -204,  -204,
    -204,  -204,   179,    38,  -204,  -204,   296,   -28,   302,   274,
    -204,  -204,  -204,    87,  -204,   318,   276,   322,   280,  -204,
      -7,  -204,   111,  -204,  -204,  -204,  -204,  -204,  -204,  -204,
    -204,    39,   319,  -204,  -204,  -204,   114,  -204,  -204,  -204,
    -204,  -204,  -204,   115,  -204,  -204,   226,  -204,  -204,  -204,
     413,  -204,  -204,   329,   288,  -204,  -204,   332,   297,  -204,
    -204,  -204,   413,   338,   303,  -204,  -204,   339,   305,  -204,
    -204,  -204,  -204,  -204,  -204,  -204
};

char **prompt_string_pointer = (char **)NULL;
int interactive = 0;
int current_token;
char *ps1_prompt, *ps2_prompt;
int parse_and_execute_level = 0;

# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
# define YYDPRINTF(Args) ((void) 0)
# define YY_ASSERT(E) ((void) (0 && (E)))
# define YY_CAST(Type, Val) ((Type) (Val))
# define YY_STACK_PRINT(Bottom, Top)
# define yypact_value_is_default(Yyn) ((Yyn) == -204)
# define prompt_is_ps1 (!prompt_string_pointer || prompt_string_pointer == &ps1_prompt)

static int yylex ()
{
	if (interactive && (current_token == 0 || current_token == '\n'))
	{
		if (prompt_is_ps1 && parse_and_execute_level == 0 && time_to_check_mail ())
		{
			check_mail ();
			reset_mail_timer ();
		}

		if (token_to_read == 0 && SHOULD_PROMPT ())
			prompt_again ();
	}

	two_tokens_ago = token_before_that;
	token_before_that = last_read_token;
	last_read_token = current_token;
	current_token = read_token (READ);

	if ((parser_state & PST_EOFTOKEN) && current_token == shell_eof_token)
	{
		current_token = yacc_EOF;
		if (bash_input.type == st_string)
			rewind_input_string ();
	}
	parser_state &= ~PST_EOFTOKEN;
	return (current_token);
}

void	ft_test(int yyn)
{
	yy_state_fast_t yystate;
	int yyerrstatus;
	long int yystacksize;
	yy_state_t yyssa[200];
	yy_state_t *yyss;
	yy_state_t *yyssp;
	YYSTYPE yyvsa[200];
	YYSTYPE *yyvs;
	YYSTYPE *yyvsp;
	int yyn;
	int yyresult;
	yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
	YYSTYPE yyval;

	#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

	int yylen = 0;
	int yynerrs = 0;
	yystate = 0;
	yyerrstatus = 0;
	yystacksize = 200;
	yyssp = yyss = yyssa;
	yyvsp = yyvs = yyvsa;


	YYDPRINTF ((stderr, "Starting parse\n"));

	int yychar = YYEMPTY;
	goto yysetstate;

	yynewstate:
		yyssp++;
	yysetstate:
		YYDPRINTF ((stderr, "Entering state %d\n", yystate));
		YY_ASSERT (0 <= yystate && yystate < 346);
		YY_IGNORE_USELESS_CAST_BEGIN
			*yyssp = YY_CAST (yy_state_t, yystate);
		YY_IGNORE_USELESS_CAST_END
		YY_STACK_PRINT (yyss, yyssp);

	if (yyss + yystacksize - 1 <= yyssp)
	#if !defined yyoverflow && !defined YYSTACK_RELOCATE
		goto yyexhaustedlab;
	#endif

	if (yystate == 118)
		goto yyacceptlab;
	goto yybackup;

	yybackup:
		yyn = yypact[yystate];
		if (yypact_value_is_default (yyn))
			goto yydefault;

		if (yychar == YYEMPTY)
		{
			YYDPRINTF ((stderr, "Reading a token\n"));
			yychar = yylex();
		}

		if (yychar <= YYEOF)
		{
			yychar = YYEOF;
			yytoken = YYSYMBOL_YYEOF;
			YYDPRINTF ((stderr, "Now at end of input.\n"));
		}
		else if (yychar == YYerror)
		{
			yychar = YYUNDEF;
			yytoken = YYSYMBOL_YYerror;
			goto yyerrlab1;
		}
		else
		{
			yytoken = YYTRANSLATE (yychar);
			YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
		}

		yyn += yytoken;
		if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
			goto yydefault;

		yyn = yytable[yyn];
		if (yyn <= 0)
		{
			if (yytable_value_is_error (yyn))
				goto yyerrlab;
			yyn = -yyn;
			goto yyreduce;
		}

		if (yyerrstatus)
			yyerrstatus--;

		YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
		yystate = yyn;
		YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
			*++yyvsp = yylval;
		YY_IGNORE_MAYBE_UNINITIALIZED_END

		yychar = YYEMPTY;
		goto yynewstate;

	yydefault:
		yyn = yydefact[yystate];
		if (yyn == 0)
			goto yyerrlab;
		goto yyreduce;

	yyreduce:
		yylen = yyr2[yyn];
		yyval = yyvsp[1-yylen];


	YY_REDUCE_PRINT (yyn);
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

	YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);
	YYPOPSTACK (yylen);
	yylen = 0;
	*++yyvsp = yyval;
	{
		const int yylhs = yyr1[yyn] - YYNTOKENS;
		const int yyi = yypgoto[yylhs] + *yyssp;
		yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp ? yytable[yyi] : yydefgoto[yylhs]);
	}

	goto yynewstate;


	yyerrlab:
		yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
		if (!yyerrstatus)
		{
			++yynerrs;
			yyerror (YY_("syntax error"));
		}

		if (yyerrstatus == 3)
		{
			if (yychar <= YYEOF)
			{
				if (yychar == YYEOF)
					YYABORT;
			}
			else
			{
				yydestruct ("Error: discarding", yytoken, &yylval);
				yychar = YYEMPTY;
			}
		}
		goto yyerrlab1;


	yyerrorlab:
		if (0)
			YYERROR;
		YYPOPSTACK (yylen);
		yylen = 0;
		YY_STACK_PRINT (yyss, yyssp);
		yystate = *yyssp;
		goto yyerrlab1;

	yyerrlab1:
		yyerrstatus = 3;

		for (;;)
		{
			yyn = yypact[yystate];
			if (!yypact_value_is_default (yyn))
			{
				yyn += YYSYMBOL_YYerror;
				if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
				{
					yyn = yytable[yyn];
					if (0 < yyn)
						break;
				}
			}
			if (yyssp == yyss)
				YYABORT;
			yydestruct ("Error: popping",
			YY_ACCESSING_SYMBOL (yystate), yyvsp);
			YYPOPSTACK (1);
			yystate = *yyssp;
			YY_STACK_PRINT (yyss, yyssp);
		}
		YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
		YY_IGNORE_MAYBE_UNINITIALIZED_END
		YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);
		yystate = yyn;
		goto yynewstate;

	yyacceptlab:
		yyresult = 0;
		goto yyreturn;

	yyabortlab:
		yyresult = 1;
		goto yyreturn;

	#if !defined yyoverflow
		yyexhaustedlab:
			yyerror (YY_("memory exhausted"));
			yyresult = 2;
	#endif

	yyreturn:
		if (yychar != YYEMPTY)
		{
			yytoken = YYTRANSLATE (yychar);
			yydestruct ("Cleanup: discarding lookahead",
			yytoken, &yylval);
		}
		YYPOPSTACK (yylen);
		YY_STACK_PRINT (yyss, yyssp);
		while (yyssp != yyss)
		{
			yydestruct ("Cleanup: popping",
			YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
			YYPOPSTACK (1);
		}
		#ifndef yyoverflow
		if (yyss != yyssa)
			YYSTACK_FREE (yyss);
		#endif

	return yyresult;

}
