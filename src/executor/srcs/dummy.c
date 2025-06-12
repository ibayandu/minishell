#include <stdlib.h>
#include <signal.h>
#include "structs.h"
#include "dummy.h"
#include "collector.h"

t_fd_bitmap				*current_fds_to_close = NULL;
volatile sig_atomic_t	terminating_signal = 0;
volatile sig_atomic_t	interrupt_state = 0;
static t_command		*currently_executing_command;
int						exit_immediately_on_error = 0;
int						command_string_index = 0;
static int				was_heredoc;
static t_redirect		*deferred_heredocs;
char					*the_printed_command = NULL;

t_fd_bitmap	*new_fd_bitmap(int size)
{
	t_fd_bitmap	*ret;

	ret = ft_malloc(sizeof(t_fd_bitmap));
	ret->size = size;
	if (size)
		ret->bitmap = ft_calloc(size, 1);
	else
		ret->bitmap = NULL;
	return (ret);
}

static void	cprintf(const char *control, va_dcl va_alist)
{
  register const char *s;
  char char_arg[2], *argp, intbuf[INT_STRLEN_BOUND (unsigned int) + 1];
  int digit_arg, arg_len, c;
  va_list args;

  SH_VA_START (args, control);

  arg_len = strlen (control);
  the_printed_command_resize (arg_len + 1);

  char_arg[1] = '\0';
  s = control;
  while (s && *s)
    {
      c = *s++;
      argp = (char *)NULL;
      if (c != '%' || !*s)
	{
	  char_arg[0] = c;
	  argp = char_arg;
	  arg_len = 1;
	}
      else
	{
	  c = *s++;
	  switch (c)
	    {
	    case '%':
	      char_arg[0] = c;
	      argp = char_arg;
	      arg_len = 1;
	      break;

	    case 's':
	      argp = va_arg (args, char *);
	      arg_len = strlen (argp);
	      break;

	    case 'd':
	      /* Represent an out-of-range file descriptor with an out-of-range
		 integer value.  We can do this because the only use of `%d' in
		 the calls to cprintf is to output a file descriptor number for
		 a redirection. */
	      digit_arg = va_arg (args, int);
	      if (digit_arg < 0)
		{
		  sprintf (intbuf, "%u", (unsigned int)-1);
		  argp = intbuf;
		}
	      else
		argp = inttostr (digit_arg, intbuf, sizeof (intbuf));
	      arg_len = strlen (argp);
	      break;

	    case 'c':
	      char_arg[0] = va_arg (args, int);
	      argp = char_arg;
	      arg_len = 1;
	      break;

	    default:
	      programming_error (_("cprintf: `%c': invalid format character"), c);
	      /*NOTREACHED*/
	    }
	}

      if (argp && arg_len)
	{
	  the_printed_command_resize (arg_len + 1);
	  FASTCOPY (argp, the_printed_command + command_string_index, arg_len);
	  command_string_index += arg_len;
	}
    }

  va_end (args);

  the_printed_command[command_string_index] = '\0';
}

static void	make_command_string_internal(t_command *command)
{
	char	s[3];

	if (command == 0)
		cprintf ("");
	else
	{
		if (skip_this_indent)
			skip_this_indent--;
		else
			indent(indentation);

      if (command->flags & CMD_TIME_PIPELINE)
	{
	  cprintf ("time ");
	  if (command->flags & CMD_TIME_POSIX)
	    cprintf ("-p ");
	}

      if (command->flags & CMD_INVERT_RETURN)
	cprintf ("! ");

      switch (command->type)
	{
	case cm_for:
	  print_for_command (command->value.For);
	  break;

#if defined (ARITH_FOR_COMMAND)
	case cm_arith_for:
	  print_arith_for_command (command->value.ArithFor);
	  break;
#endif

#if defined (SELECT_COMMAND)
	case cm_select:
	  print_select_command (command->value.Select);
	  break;
#endif

	case cm_case:
	  print_case_command (command->value.Case);
	  break;

	case cm_while:
	  print_while_command (command->value.While);
	  break;

	case cm_until:
	  print_until_command (command->value.While);
	  break;

	case cm_if:
	  print_if_command (command->value.If);
	  break;

#if defined (DPAREN_ARITHMETIC)
	case cm_arith:
	  print_arith_command (command->value.Arith->exp);
	  break;
#endif

#if defined (COND_COMMAND)
	case cm_cond:
	  print_cond_command (command->value.Cond);
	  break;
#endif

	case cm_simple:
	  print_simple_command (command->value.Simple);
	  break;

	case cm_connection:

	  skip_this_indent++;
	  printing_connection++;
	  make_command_string_internal (command->value.Connection->first);

	  switch (command->value.Connection->connector)
	    {
	    case '&':
	    case '|':
	      {
		char c = command->value.Connection->connector;

		s[0] = ' ';
		s[1] = c;
		s[2] = '\0';

		print_deferred_heredocs (s);

		if (c != '&' || command->value.Connection->second)
		  {
		    cprintf (" ");
		    skip_this_indent++;
		  }
	      }
	      break;

	    case AND_AND:
	      print_deferred_heredocs (" && ");
	      if (command->value.Connection->second)
		skip_this_indent++;
	      break;

	    case OR_OR:
	      print_deferred_heredocs (" || ");
	      if (command->value.Connection->second)
		skip_this_indent++;
	      break;

	    case ';':
	    case '\n':				/* special case this */
	      {
		char c = command->value.Connection->connector;
		int was_newline;

		s[0] = printing_comsub ? c : ';';
		s[1] = '\0';

		was_newline = deferred_heredocs == 0 && was_heredoc == 0 && c == '\n';
		if (deferred_heredocs == 0)
		  {
		    if (was_heredoc == 0)
		      cprintf ("%s", s);	/* inside_function_def? */
		    else
		      was_heredoc = 0;
		  }
		else
		  /* print_deferred_heredocs special-cases `;' */
		  print_deferred_heredocs (inside_function_def ? "" : ";");

		if (inside_function_def)
		  cprintf ("\n");
		else if (printing_comsub && c == '\n' && was_newline == 0)
		  cprintf ("\n");	/* preserve newlines in comsubs but don't double them */
		else
		  {
		    if (c == ';')
		      cprintf (" ");
		    if (command->value.Connection->second)
		      skip_this_indent++;
		  }
		break;
	      }

	    default:
	      cprintf (_("print_command: bad connector `%d'"),
		       command->value.Connection->connector);
	      break;
	    }

	  make_command_string_internal (command->value.Connection->second);
	  PRINT_DEFERRED_HEREDOCS ("");
	  printing_connection--;
	  break;

	case cm_function_def:
	  print_function_def (command->value.Function_def);
	  break;

	case cm_group:
	  print_group_command (command->value.Group);
	  break;

	case cm_subshell:
	  cprintf ("( ");
	  skip_this_indent++;
	  make_command_string_internal (command->value.Subshell->command);
	  PRINT_DEFERRED_HEREDOCS ("");
	  cprintf (" )");
	  break;

	case cm_coproc:
	  cprintf ("coproc ");
	  if (command->value.Coproc->command->type != cm_simple)
	    cprintf ("%s ", command->value.Coproc->name);
	  skip_this_indent++;
	  make_command_string_internal (command->value.Coproc->command);
	  break;

	default:
	  command_error ("print_command", CMDERR_BADTYPE, command->type, 0);
	  break;
	}


      if (command->redirects)
	{
	  cprintf (" ");
	  print_redirection_list (command->redirects);
	}
    }
}

char	*make_command_string(t_command *command)
{
	command_string_index = 0;
	was_heredoc = 0;
	deferred_heredocs = 0;
	make_command_string_internal(command);
	return (the_printed_command);
}


int	execute_command_internal(t_command *command, int asynchronous, int pipe_in, int pipe_out, t_fd_bitmap *fds_to_close)
{
	int				exec_result;
	int				user_subshell;
	int				invert;
	int				ignore_return;
	int				was_error_trap;
	int				fork_flags;
	int				paren_pid;
	int				s;
	t_redirect		*my_undo_list;
	t_redirect		*exec_undo_list;
	char			*tcmd;
	char			*p;


	if (command == 0)
		return (EXECUTION_SUCCESS);
	if (terminating_signal)
		termsig_handler(terminating_signal);
	if (interrupt_state)
		throw_to_top_level();
	run_pending_traps(); // It handles bash scripting signals, not necessary but might be best practice
	currently_executing_command = command;
	invert = (command->flags & CMD_INVERT_RETURN) != 0;
	if (exit_immediately_on_error && invert)
		command->flags |= CMD_IGNORE_RETURN;
	exec_result = EXECUTION_SUCCESS;
	if (command->type == CMD_SUBSHELL && (command->flags & CMD_NO_FORK))
		return (execute_in_subshell(command, asynchronous, pipe_in, pipe_out, fds_to_close));
	user_subshell = command->type == CMD_SUBSHELL || ((command->flags & CMD_WANT_SUBSHELL) != 0);
	if (command->type == CMD_SUBSHELL || (command->flags & (CMD_WANT_SUBSHELL | CMD_FORCE_SUBSHELL)))
	{
		/* Fork a subshell, turn off the subshell bit, turn off job
		control and call execute_command () on the command again. */
		/* Otherwise we defer setting line_number */
		tcmd = make_command_string(command);
		fork_flags = asynchronous ? FORK_ASYNC : 0;
		paren_pid = make_child (p = savestring (tcmd), fork_flags);

		if (user_subshell && signal_is_trapped (ERROR_TRAP) &&
		signal_in_progress (DEBUG_TRAP) == 0 && running_trap == 0)
		{
		FREE (the_printed_command_except_trap);
		the_printed_command_except_trap = savestring (the_printed_command);
		}

		if (paren_pid == 0)
		{
		#if defined (JOB_CONTROL)
		FREE (p);		/* child doesn't use pointer */
		#endif
		/* We want to run the exit trap for forced {} subshells, and we
		want to note this before execute_in_subshell modifies the
		COMMAND struct.  Need to keep in mind that execute_in_subshell
		runs the exit trap for () subshells itself. */
		/* This handles { command; } & */
		s = user_subshell == 0 && command->type == cm_group && pipe_in == NO_PIPE && pipe_out == NO_PIPE && asynchronous;
		/* run exit trap for : | { ...; } and { ...; } | : */
		/* run exit trap for : | ( ...; ) and ( ...; ) | : */
		s += user_subshell == 0 && command->type == cm_group && (pipe_in != NO_PIPE || pipe_out != NO_PIPE) && asynchronous == 0;

		last_command_exit_value = execute_in_subshell (command, asynchronous, pipe_in, pipe_out, fds_to_close);
		if (s)
		subshell_exit (last_command_exit_value);
		else
		sh_exit (last_command_exit_value);
		/* NOTREACHED */
		}
		else
		{
		close_pipes (pipe_in, pipe_out);

		#if defined (PROCESS_SUBSTITUTION) && defined (HAVE_DEV_FD)
		if (variable_context == 0)	/* wait until shell function completes */
		unlink_fifo_list ();
		#endif
		/* If we are part of a pipeline, and not the end of the pipeline,
		then we should simply return and let the last command in the
		pipe be waited for.  If we are not in a pipeline, or are the
		last command in the pipeline, then we wait for the subshell
		and return its exit status as usual. */
		if (pipe_out != NO_PIPE)
		return (EXECUTION_SUCCESS);

		stop_pipeline (asynchronous, (COMMAND *)NULL);

		line_number = save_line_number;

		if (asynchronous == 0)
		{
		was_error_trap = signal_is_trapped (ERROR_TRAP) && signal_is_ignored (ERROR_TRAP) == 0;
		invert = (command->flags & CMD_INVERT_RETURN) != 0;
		ignore_return = (command->flags & CMD_IGNORE_RETURN) != 0;

		exec_result = wait_for (paren_pid, 0);

		/* If we have to, invert the return value. */
		if (invert)
		exec_result = ((exec_result == EXECUTION_SUCCESS)
		? EXECUTION_FAILURE
		: EXECUTION_SUCCESS);

		last_command_exit_value = exec_result;
		if (user_subshell && was_error_trap && ignore_return == 0 && invert == 0 && exec_result != EXECUTION_SUCCESS)
		{
		save_line_number = line_number;
		line_number = line_number_for_err_trap;
		run_error_trap ();
		line_number = save_line_number;
		}

		if (user_subshell && ignore_return == 0 && invert == 0 && exit_immediately_on_error && exec_result != EXECUTION_SUCCESS)
		{
		run_pending_traps ();
		jump_to_top_level (ERREXIT);
		}

		return (last_command_exit_value);
		}
		else
		{
		DESCRIBE_PID (paren_pid);

		run_pending_traps ();

		/* Posix 2013 2.9.3.1: "the exit status of an asynchronous list
		shall be zero." */
		last_command_exit_value = 0;
		return (EXECUTION_SUCCESS);
		}
		}
	}

#if defined (COMMAND_TIMING)
  if (command->flags & CMD_TIME_PIPELINE)
    {
      if (asynchronous)
	{
	  command->flags |= CMD_FORCE_SUBSHELL;
	  exec_result = execute_command_internal (command, 1, pipe_in, pipe_out, fds_to_close);
	}
      else
	{
	  exec_result = time_command (command, asynchronous, pipe_in, pipe_out, fds_to_close);
#if 0
	  if (running_trap == 0)
#endif
	    currently_executing_command = (COMMAND *)NULL;
	}
      return (exec_result);
    }
#endif /* COMMAND_TIMING */

  if (shell_control_structure (command->type) && command->redirects)
    stdin_redir = stdin_redirects (command->redirects);

#if defined (PROCESS_SUBSTITUTION)
#  if !defined (HAVE_DEV_FD)
  reap_procsubs ();
#  endif

  /* XXX - also if sourcelevel != 0? */
  if (variable_context != 0 || executing_list)
    {
      ofifo = num_fifos ();
      ofifo_list = copy_fifo_list ((int *)&osize);
      begin_unwind_frame ("internal_fifos");
      if (ofifo_list)
	add_unwind_protect (xfree, ofifo_list);
      saved_fifo = 1;
    }
  else
    saved_fifo = 0;
#endif

  /* Handle WHILE FOR CASE etc. with redirections.  (Also '&' input
     redirection.)  */
  was_error_trap = signal_is_trapped (ERROR_TRAP) && signal_is_ignored (ERROR_TRAP) == 0;
  ignore_return = (command->flags & CMD_IGNORE_RETURN) != 0;

  if (do_redirections (command->redirects, RX_ACTIVE|RX_UNDOABLE) != 0)
    {
      undo_partial_redirects ();
      dispose_exec_redirects ();
#if defined (PROCESS_SUBSTITUTION)
      if (saved_fifo)
	{
	  free ((void *)ofifo_list);
          discard_unwind_frame ("internal_fifos");
	}
#endif

      /* Handle redirection error as command failure if errexit set. */
      last_command_exit_value = EXECUTION_FAILURE;
      if (ignore_return == 0 && invert == 0 && pipe_in == NO_PIPE && pipe_out == NO_PIPE)
	{
	  if (was_error_trap)
	    {
	      save_line_number = line_number;
	      line_number = line_number_for_err_trap;
	      run_error_trap ();
	      line_number = save_line_number;
	    }
	  if (exit_immediately_on_error)
	    {
	      run_pending_traps ();
	      jump_to_top_level (ERREXIT);
	    }
	}
      return (last_command_exit_value);
    }

  my_undo_list = redirection_undo_list;
  redirection_undo_list = (REDIRECT *)NULL;

  exec_undo_list = exec_redirection_undo_list;
  exec_redirection_undo_list = (REDIRECT *)NULL;

  if (my_undo_list || exec_undo_list)
    begin_unwind_frame ("loop_redirections");

  if (my_undo_list)
    add_unwind_protect ((Function *)cleanup_redirects, my_undo_list);

  if (exec_undo_list)
    add_unwind_protect ((Function *)dispose_redirects, exec_undo_list);

  QUIT;

  switch (command->type)
    {
    case cm_simple:
      {
	save_line_number = line_number;
	/* We can't rely on variables retaining their values across a
	   call to execute_simple_command if a longjmp occurs as the
	   result of a `return' builtin.  This is true for sure with gcc. */
#if defined (RECYCLES_PIDS)
	last_made_pid = NO_PID;
#endif
	was_error_trap = signal_is_trapped (ERROR_TRAP) && signal_is_ignored (ERROR_TRAP) == 0;

	if (ignore_return && command->value.Simple)
	  command->value.Simple->flags |= CMD_IGNORE_RETURN;
	if (command->flags & CMD_STDIN_REDIR)
	  command->value.Simple->flags |= CMD_STDIN_REDIR;

	SET_LINE_NUMBER (command->value.Simple->line);
	exec_result =
	  execute_simple_command (command->value.Simple, pipe_in, pipe_out,
				  asynchronous, fds_to_close);
	line_number = save_line_number;

	/* The temporary environment should be used for only the simple
	   command immediately following its definition. */
	dispose_used_env_vars ();

#if (defined (ultrix) && defined (mips)) || defined (C_ALLOCA)
	/* Reclaim memory allocated with alloca () on machines which
	   may be using the alloca emulation code. */
	(void) alloca (0);
#endif /* (ultrix && mips) || C_ALLOCA */

	/* If we forked to do the command, then we must wait_for ()
	   the child. */

	/* XXX - this is something to watch out for if there are problems
	   when the shell is compiled without job control.  Don't worry about
	   whether or not last_made_pid == last_pid; already_making_children
	   tells us whether or not there are unwaited-for children to wait
	   for and reap. */
	if (already_making_children && pipe_out == NO_PIPE)
	  {
	    stop_pipeline (asynchronous, (COMMAND *)NULL);

	    if (asynchronous)
	      {
		DESCRIBE_PID (last_made_pid);
		exec_result = EXECUTION_SUCCESS;
		invert = 0;		/* async commands always succeed */
	      }
	    else
#if !defined (JOB_CONTROL)
	      /* Do not wait for asynchronous processes started from
		 startup files. */
	    if (last_made_pid != NO_PID && last_made_pid != last_asynchronous_pid)
#else
	    if (last_made_pid != NO_PID)
#endif
	    /* When executing a shell function that executes other
	       commands, this causes the last simple command in
	       the function to be waited for twice.  This also causes
	       subshells forked to execute builtin commands (e.g., in
	       pipelines) to be waited for twice. */
	      exec_result = wait_for (last_made_pid, 0);
	  }
      }

      /* 2009/02/13 -- pipeline failure is processed elsewhere.  This handles
	 only the failure of a simple command. We don't want to run the error
	 trap if the command run by the `command' builtin fails; we want to
	 defer that until the command builtin itself returns failure. */
      /* 2020/07/14 -- this changes with how the command builtin is handled */
      if (was_error_trap && ignore_return == 0 && invert == 0 &&
	    pipe_in == NO_PIPE && pipe_out == NO_PIPE &&
	    (command->value.Simple->flags & CMD_COMMAND_BUILTIN) == 0 &&
	    exec_result != EXECUTION_SUCCESS)
	{
	  last_command_exit_value = exec_result;
	  line_number = line_number_for_err_trap;
	  run_error_trap ();
	  line_number = save_line_number;
	}

      if (ignore_return == 0 && invert == 0 &&
	  ((posixly_correct && interactive == 0 && special_builtin_failed) ||
	   (exit_immediately_on_error && pipe_in == NO_PIPE && pipe_out == NO_PIPE && exec_result != EXECUTION_SUCCESS)))
	{
	  last_command_exit_value = exec_result;
	  run_pending_traps ();

	  /* Undo redirections before running exit trap on the way out of
	     set -e. Report by Mark Farrell 5/19/2014 */
	  if (exit_immediately_on_error && signal_is_trapped (0) &&
		unwind_protect_tag_on_stack ("saved-redirects"))
	    run_unwind_frame ("saved-redirects");

	  jump_to_top_level (ERREXIT);
	}

      break;

    case cm_for:
      if (ignore_return)
	command->value.For->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_for_command (command->value.For);
      break;

#if defined (ARITH_FOR_COMMAND)
    case cm_arith_for:
      if (ignore_return)
	command->value.ArithFor->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_arith_for_command (command->value.ArithFor);
      break;
#endif

#if defined (SELECT_COMMAND)
    case cm_select:
      if (ignore_return)
	command->value.Select->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_select_command (command->value.Select);
      break;
#endif

    case cm_case:
      if (ignore_return)
	command->value.Case->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_case_command (command->value.Case);
      break;

    case cm_while:
      if (ignore_return)
	command->value.While->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_while_command (command->value.While);
      break;

    case cm_until:
      if (ignore_return)
	command->value.While->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_until_command (command->value.While);
      break;

    case cm_if:
      if (ignore_return)
	command->value.If->flags |= CMD_IGNORE_RETURN;
      exec_result = execute_if_command (command->value.If);
      break;

    case cm_group:

      /* This code can be executed from either of two paths: an explicit
	 '{}' command, or via a function call.  If we are executed via a
	 function call, we have already taken care of the function being
	 executed in the background (down there in execute_simple_command ()),
	 and this command should *not* be marked as asynchronous.  If we
	 are executing a regular '{}' group command, and asynchronous == 1,
	 we must want to execute the whole command in the background, so we
	 need a subshell, and we want the stuff executed in that subshell
	 (this group command) to be executed in the foreground of that
	 subshell (i.e. there will not be *another* subshell forked).

	 What we do is to force a subshell if asynchronous, and then call
	 execute_command_internal again with asynchronous still set to 1,
	 but with the original group command, so the printed command will
	 look right.

	 The code above that handles forking off subshells will note that
	 both subshell and async are on, and turn off async in the child
	 after forking the subshell (but leave async set in the parent, so
	 the normal call to describe_pid is made).  This turning off
	 async is *crucial*; if it is not done, this will fall into an
	 infinite loop of executions through this spot in subshell after
	 subshell until the process limit is exhausted. */

      if (asynchronous)
	{
	  command->flags |= CMD_FORCE_SUBSHELL;
	  exec_result =
	    execute_command_internal (command, 1, pipe_in, pipe_out,
				      fds_to_close);
	}
      else
	{
	  if (ignore_return && command->value.Group->command)
	    command->value.Group->command->flags |= CMD_IGNORE_RETURN;
	  exec_result =
	    execute_command_internal (command->value.Group->command,
				      asynchronous, pipe_in, pipe_out,
				      fds_to_close);
	}
      break;

    case cm_connection:
      exec_result = execute_connection (command, asynchronous,
					pipe_in, pipe_out, fds_to_close);
      if (asynchronous)
	invert = 0;		/* XXX */

      break;

#if defined (DPAREN_ARITHMETIC)
    case cm_arith:
#endif
#if defined (COND_COMMAND)
    case cm_cond:
#endif
    case cm_function_def:
      was_error_trap = signal_is_trapped (ERROR_TRAP) && signal_is_ignored (ERROR_TRAP) == 0;
#if defined (DPAREN_ARITHMETIC)
      if (ignore_return && command->type == cm_arith)
	command->value.Arith->flags |= CMD_IGNORE_RETURN;
#endif
#if defined (COND_COMMAND)
      if (ignore_return && command->type == cm_cond)
	command->value.Cond->flags |= CMD_IGNORE_RETURN;
#endif

      line_number_for_err_trap = save_line_number = line_number;	/* XXX */
#if defined (DPAREN_ARITHMETIC)
      if (command->type == cm_arith)
	exec_result = execute_arith_command (command->value.Arith);
      else
#endif
#if defined (COND_COMMAND)
      if (command->type == cm_cond)
	exec_result = execute_cond_command (command->value.Cond);
      else
#endif
      if (command->type == cm_function_def)
	exec_result = execute_intern_function (command->value.Function_def->name,
					       command->value.Function_def);
      line_number = save_line_number;

      if (was_error_trap && ignore_return == 0 && invert == 0 && exec_result != EXECUTION_SUCCESS)
	{
	  last_command_exit_value = exec_result;
	  save_line_number = line_number;
	  line_number = line_number_for_err_trap;
	  run_error_trap ();
	  line_number = save_line_number;
	}

      if (ignore_return == 0 && invert == 0 && exit_immediately_on_error && exec_result != EXECUTION_SUCCESS)
	{
	  last_command_exit_value = exec_result;
	  run_pending_traps ();
	  jump_to_top_level (ERREXIT);
	}

      break;

    default:
      command_error ("execute_command", CMDERR_BADTYPE, command->type, 0);
    }

  if (my_undo_list)
    cleanup_redirects (my_undo_list);

  if (exec_undo_list)
    dispose_redirects (exec_undo_list);

  if (my_undo_list || exec_undo_list)
    discard_unwind_frame ("loop_redirections");

#if defined (PROCESS_SUBSTITUTION)
  if (saved_fifo)
    {
      nfifo = num_fifos ();
      if (nfifo > ofifo)
	close_new_fifos ((void *)ofifo_list, osize);
      free ((void *)ofifo_list);
      discard_unwind_frame ("internal_fifos");
    }
#endif

  /* Invert the return value if we have to */
  if (invert)
    exec_result = (exec_result == EXECUTION_SUCCESS)
		    ? EXECUTION_FAILURE
		    : EXECUTION_SUCCESS;

#if defined (DPAREN_ARITHMETIC) || defined (COND_COMMAND)
  /* This is where we set PIPESTATUS from the exit status of the appropriate
     compound commands (the ones that look enough like simple commands to
     cause confusion).  We might be able to optimize by not doing this if
     subshell_environment != 0. */
  switch (command->type)
    {
#  if defined (DPAREN_ARITHMETIC)
    case cm_arith:
#  endif
#  if defined (COND_COMMAND)
    case cm_cond:
#  endif
      set_pipestatus_from_exit (exec_result);
      break;
    default:
      break;
    }
#endif

  last_command_exit_value = exec_result;
  run_pending_traps ();
  currently_executing_command = (COMMAND *)NULL;

  return (last_command_exit_value);
}

int	execute_command(t_command *command)
{
	t_fd_bitmap	*bitmap;
	int			result;

	current_fds_to_close = NULL;
	bitmap = new_fd_bitmap (32);
	result = execute_command_internal(command, 0, NO_PIPE, NO_PIPE, bitmap);
	if (terminating_signal)
		termsig_handler(terminating_signal);
	if (interrupt_state)
		throw_to_top_level();
	return (result);
}
