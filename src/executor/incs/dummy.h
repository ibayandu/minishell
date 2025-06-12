#ifndef DUMMY_H
# define DUMMY_H

# define NO_PIPE -1
# define EXECUTION_SUCCESS 0

# define CMD_INVERT_RETURN 0x04
# define CMD_IGNORE_RETURN 0x08
# define CMD_NO_FORK 0x40
# define CMD_WANT_SUBSHELL 0x01
# define CMD_FORCE_SUBSHELL 0x02


typedef struct s_fd_bitmap
{
	int		size;
	char	*bitmap;

} t_fd_bitmap;

#endif // DUMMY_H
