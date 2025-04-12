/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 17:24:53 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

int	main(void)
{
	char			*input;
	t_token_list	*token_list;

	input = "#!/bin/bash\n\
# Simple line count example, using bash\n\
#\n\
# Bash tutorial: http://linuxconfig.org/Bash_scripting_Tutorial#8-2-read-file-into-bash-array\n\
# My scripting link: http://www.macs.hw.ac.uk/~hwloidl/docs/index.html#scripting\n\
#\n\
# Usage: ./line_count.sh file\n\
# -----------------------------------------------------------------------------\n\
\n\
# Link filedescriptor 10 with stdin\n\
exec 10<&0\n\
#stdin replaced with a file supplied as a first argument\n\
exec < $1\n\
#remember the name of the input file\n\
\n\
\n\
#init \n\
file \"current_line.txt\"\n\
let count=0\n\
\n\
#this while loop iterates over all lines of the file\n\
while read LINE # deneme\n\
do\n\
    # increase line counter \n\
    ((count++))\n\
    # write current line to a tmp file with name $file (not needed for counting)\n\
    echo $LINE > $file\n\
    # this checks the return (code of echo (not needed for writing); just for demo)\n\
    if [ $? -ne 0 ] \n\
     then echo \"Error in writing to file ${file}; check its permissions!\"\n\
    fi\n\
done\n\
\n\
echo \"Number of lines: $count\"\n\
echo \"The last line of the file is: `cat ${file}`\"\n\
\n\
# Note: You can achieve the same by just using the tool wc like this\n\
echo \"Expected number of lines: `wc -l $in`\"\n\
\n\
<<-&-\n\
#restore stdin from filedescriptor 10\n\
#and close filedescriptor 10\n\
exec 0<&10 10<&-\n";
	token_list = lexer(input);
	ft_putnbr_fd(token_list->count, 1);
}
