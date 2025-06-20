#include "makers.h"
#include "libft.h"
#include "expander.h"

t_word_list	*glob_expand_word_list(t_word_list *tlist)
{
	char		**glob_array;
	char		*temp_string;
	int			glob_index;
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*disposables;
	t_word_list	*next;
	t_word		*tword;

	output_list = NULL;
	disposables = NULL;
	glob_array = NULL;
	while (tlist)
	{
		next = tlist->next;
		if ((tlist->word->flags & W_NOGLOB) == 0 && unquoted_glob_pattern_p (tlist->word->word))
		{
			glob_array = shell_glob_filename (tlist->word->word, QGLOB_CTLESC);
			if (glob_array == 0 || GLOB_FAILED(glob_array))
			{
				glob_array = ft_malloc(sizeof (char *));
				glob_array[0] = NULL;
			}
			if (glob_array[0] == NULL)
			{
				temp_string = dequote_string (tlist->word->word);
				tlist->word->word = temp_string;
			}
			glob_list = NULL;
			glob_index = 0;
			while (glob_array[glob_index])
			{
				tword = make_bare_word(glob_array[glob_index]);
				glob_list = make_word_list(tword, glob_list);
				glob_index++;
			}

			if (glob_list)
			{
				output_list = list_append(glob_list, output_list);
				PREPEND_LIST (tlist, disposables);
			}
			else if (fail_glob_expansion)
			{
				last_command_exit_value = EXECUTION_FAILURE;
				report_error (_("no match: %s"), tlist->word->word);
				exp_jump_to_top_level (DISCARD);
			}
			else if (!allow_null_glob_expansion)
				PREPEND_LIST (tlist, output_list);
			else
				PREPEND_LIST (tlist, disposables);
		}
		else
		{
			temp_string = dequote_string (tlist->word->word);
			tlist->word->word = temp_string;
			PREPEND_LIST (tlist, output_list);
		}
		strvec_dispose (glob_array);
		glob_array = NULL;
		tlist = next;
	}
	if (output_list)
		output_list = ft_revword (output_list);
	return (output_list);
}
