#include "structs.h"
#include "libft.h"

t_word	*param_expand (char *string, int *sindex, int quoted, int *expanded_something, int *contains_dollar_at, int *quoted_dollar_at_p, int *had_quoted_null_p, int pflags)
{
	char			*temp;
	char			*temp1;
	int				zindex;
	int				t_index;
	t_variable		*var;
	t_word			*ret;
	int				tflag;

	zindex = *sindex;
	temp = NULL;
	ret = NULL;
	tflag = 0;
	switch (string[++zindex])
	{
		case '?':
			temp = ft_itoa(last_command_exit_value);
			break;
		default:
			temp = (char *)NULL;
			t_index = zindex;
			while (string[zindex] && legal_variable_char(string[zindex]))
				zindex++;
			if (zindex > t_index)
				temp1 = ft_substr(string, t_index, zindex);
			else
				temp1 = NULL;
			if (!temp1 || !*temp1)
			{
				temp = ft_strdup("$");
				if (expanded_something)
					*expanded_something = 0;
				goto return0;
			}
			var = find_variable(temp1);
			if (var && invisible_p (var) == 0 && var_isset (var))
			{
				temp = value_cell(var);
				temp = (*temp && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))) ? quote_string (temp) : ((pflags & PF_ASSIGNRHS) ? quote_rhs (temp) : quote_escapes (temp));
				goto return0;
			}
			else if (var && (invisible_p (var) || var_isset (var) == 0))
				temp = (char *)NULL;
			else if ((var = find_variable_last_nameref (temp1, 0)) && var_isset (var) && invisible_p (var) == 0)
			{
				temp = nameref_cell (var);
				if (temp && *temp && legal_identifier (temp) == 0)
				{
					set_exit_status (EXECUTION_FAILURE);
					report_error (_("%s: invalid variable name for name reference"), temp);
					return (&expand_wdesc_error);	/* XXX */
				}
				else
					temp = (char *)NULL;
			}
			temp = (char *)NULL;
			unbound_variable:
			if (unbound_vars_is_error)
			{
				set_exit_status (EXECUTION_FAILURE);
				err_unboundvar (temp1);
			}
			else
				goto return0;
			set_exit_status (EXECUTION_FAILURE);
			return ((unbound_vars_is_error && interactive_shell == 0) ? &expand_wdesc_fatal : &expand_wdesc_error);
	}
	if (string[zindex])
		zindex++;
	return0:
	*sindex = zindex;
	if (ret == 0)
	{
		ret = alloc_word_desc ();
		ret->flags = tflag;
		ret->word = temp;
	}
	return ret;
}
