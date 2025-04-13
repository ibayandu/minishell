#include <stdio.h>
#include "incs/collector.h"

typedef struct s_program
{
	t_memblock *head;

	// other stuff about program...

}	t_program;

// Exiting program never been this easy before!!!
void	ft_exit(t_memblock **head, int status)
{
	ft_free(head);
	exit(status);
}

/*
* One of the cons about this approach on allocation is that we should
* give the program struct everywhere that use collector calls (like ft_malloc,
* ft_realloc, ...). We can also choose to give just &program->head to those
* functions but there is no actual differents. Since the program struct is
* something that covers everything about our logic, we may also choose to give
* every function no matter what.
*/
char	*ft_new_strdup(t_program *program, char *str)
{
	char	*res;
	size_t	i;
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	res = ft_malloc(&program->head, sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;

	srclen = 0;
	while (src[srclen])
		srclen++;
	if (!dstsize)
		return (srclen);
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize)
		dst[i] = '\0';
	return (srclen);
}

// Compile like this: cc example.c ../build/libs/collector.a -I ..
int	main(void)
{
	// Initilazing everything zero inside the program
	t_program *const	program = &(t_program){0};

	// New implementation of strdup
	char *str = ft_new_strdup(program, "test");
	if (!str)
		// We can handle malloc error like this
		ft_exit(&program->head, 1);

	printf("str: %s\n", str);

	char *new = ft_realloc(&program->head, str, 10);

	printf("same str: %s\n", new);

	// but I can now safely change
	new[7] = 'a';
	printf("and I can reach: %c\n", new[7]);

	// After everthing finish, we can just free all the nodes in one call. No need another free.
	ft_free(&program->head);
	return (0);
}
