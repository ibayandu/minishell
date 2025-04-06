/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 05:32:19 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/includes/token_utils.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_tokens	*tokens;
	char	*input;
	int		i;

	input = " \n \n #!/bin/bash\n\
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
	tokens = lexer(input);
	i = 0;
	while (tokens->tokens[i].value)
	{
		printf("Token: %s , Type: %d\n", tokens->tokens[i].value, tokens->tokens[i].type);
		i++;
	}
	printf("Token: %s , Type: %d\n", tokens->tokens[i].value, tokens->tokens[i].type);
	
	printf("\n\n_____________________________AST_NODE_____________________________\n");




	ASTNode *ast_node = parse(tokens);
	print_ast(ast_node,10);
}
