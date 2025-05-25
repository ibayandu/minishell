/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 14:45:43 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int g_last_exit_status = 0;

int execute_ast(t_command *ast_root, char ***envp_ptr)
{
	int	original_stdin_val =  dup(STDIN_FILENO);
	int	original_stdout_val = dup(STDOUT_FILENO);

	if (!ast_root)
		return 0;
	if (original_stdin_val == -1 || original_stdout_val == -1)
	{
		perror("executor: dup original stdio failed");
		if (original_stdin_val != -1)
			close(original_stdin_val);
		if (original_stdout_val != -1)
			close(original_stdout_val);
		return 127;
	}
	execute_command_node(ast_root, *envp_ptr, &original_stdin_val, &original_stdout_val);
	if (dup2(original_stdin_val, STDIN_FILENO) == -1)
		perror("executor: failed to restore stdin");
	if (dup2(original_stdout_val, STDOUT_FILENO) == -1)
		perror("executor: failed to restore stdout");
	close(original_stdin_val);
	close(original_stdout_val);
	return g_last_exit_status;
}

int execute_command_node(t_command *command_node, char **envp, int *original_stdin, int *original_stdout)
{
	if (!command_node)
		return 0;
	if (command_node->type == CMD_SIMPLE)
		return execute_simple_command(command_node->value.simple, envp);
	else if (command_node->type == CMD_CONNECT)
		return execute_connected_command(command_node->value.connection, envp, original_stdin, original_stdout);
	else if (command_node->type == CMD_SUBSHELL)
		return execute_subshell_command(command_node->value.subshell, command_node->redirects, envp);
	else
	{
		fprintf(stderr, "executor: Unknown command type %d\n", command_node->type);
		g_last_exit_status = 127;
		return 1;
	}
}

int execute_simple_command(t_simple_cmd *simple_cmd,  char **envp)
{
	pid_t	pid;
	int		status;
	char	**argv;
	char	*command_path;
	int		saved_fds[10] = {-1};
	int		heredoc_temp_fds[2] = {-1, -1};
	int		builtin_ret;

	if (!simple_cmd || !simple_cmd->words)
	{
		if (simple_cmd && handle_redirections(simple_cmd->redirects, saved_fds, heredoc_temp_fds))
		{
			if(heredoc_temp_fds[0] != -1)
				close(heredoc_temp_fds[0]);
			if(heredoc_temp_fds[1] != -1)
				close(heredoc_temp_fds[1]);
			g_last_exit_status = 1;
			return 1;
		}
		if(heredoc_temp_fds[0] != -1)
			close(heredoc_temp_fds[0]);
		if(heredoc_temp_fds[1] != -1)
			close(heredoc_temp_fds[1]);
		g_last_exit_status = 0;
		return 0;
	}
	argv = build_argv(simple_cmd->words);
	if (!argv)
	{
		g_last_exit_status = 127;
		return 1;
	}
	builtin_ret = execute_builtin(argv);
	if (builtin_ret != 0)
	{
		if (builtin_ret == 1)
			g_last_exit_status = 0;
		else
			g_last_exit_status = builtin_ret;
		if (handle_redirections(simple_cmd->redirects, saved_fds, heredoc_temp_fds))
			g_last_exit_status = 1;
		if(heredoc_temp_fds[0] != -1)
			close(heredoc_temp_fds[0]);
		if(heredoc_temp_fds[1] != -1)
			close(heredoc_temp_fds[1]);
		free_argv(argv);
		return (g_last_exit_status == 0) ? 0 : 1;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("executor: fork failed");
		free_argv(argv);
		g_last_exit_status = 127;
		return 1;
	}

	if (pid == 0)
	{
		if (handle_redirections(simple_cmd->redirects, NULL, heredoc_temp_fds))
		{
			if(heredoc_temp_fds[1] != -1)
				close(heredoc_temp_fds[1]);
			exit(1);
		}
		if(heredoc_temp_fds[0] != -1 && (heredoc_temp_fds[0] != STDIN_FILENO && simple_cmd->redirects->redirector)) {//TODO
		// This logic is tricky: if heredoc_temp_fds[0] became the new STDIN_FILENO,
		// we don't close it. If it was duped to another fd that then became STDIN_FILENO,
		// then heredoc_temp_fds[0] can be closed.
		// Simpler: the fd that was set as redirector.fd from make_here_document is now used.
		// And make_here_document should have closed the write end of its pipe.
		}
		command_path = resolve_command_path(argv[0]);
		if (!command_path)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			exit(127);
		}
		execve(command_path, argv, envp);
		perror(argv[0]);
		exit(126);
	}
	else
	{
		if (heredoc_temp_fds[1] != -1)
		{
			close(heredoc_temp_fds[1]);
			heredoc_temp_fds[1] = -1;
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
				g_last_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				fprintf(stdout, "\n");
		}
		if (heredoc_temp_fds[0] != -1)
			close(heredoc_temp_fds[0]);
		free_argv(argv);
	}
	return (!(!g_last_exit_status));
}


int execute_connected_command(t_connect_cmd *connect_cmd, char **envp, int *original_stdin, int *original_stdout) {
    if (connect_cmd->type == CNT_PIPE) {
        int pipefd[2];
        pid_t pid1, pid2;
        int status1, status2;

        if (pipe(pipefd) == -1) {
            perror("executor: pipe failed");
            g_last_exit_status = 1;
            return 1;
        }

        pid1 = fork(); // For first command (writer to pipe)
        if (pid1 == -1) {
            perror("executor: fork for pipe failed");
            close(pipefd[0]); close(pipefd[1]);
            g_last_exit_status = 1;
            return 1;
        }

        if (pid1 == 0) { // Child 1 (first command)
            close(pipefd[0]);    // Close unused read end
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("executor: dup2 stdout to pipe failed");
                close(pipefd[1]);
                exit(1);
            }
            close(pipefd[1]);    // FD is duplicated, close original

            // Recursively execute the first command. It will handle its own internal state.
            // Pass original_stdin and a placeholder for original_stdout as it's now the pipe.
            execute_command_node(connect_cmd->first, envp, original_stdin, &pipefd[1] /* not really used as original_stdout by child */);
            exit(g_last_exit_status); // Exit with the status of connect_cmd->first
        }

        pid2 = fork(); // For second command (reader from pipe)
        if (pid2 == -1) {
            perror("executor: fork for pipe failed");
            close(pipefd[0]); close(pipefd[1]);
            waitpid(pid1, NULL, 0); // Reap first child
            g_last_exit_status = 1;
            return 1;
        }

        if (pid2 == 0) { // Child 2 (second command)
            close(pipefd[1]);    // Close unused write end
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                perror("executor: dup2 stdin from pipe failed");
                close(pipefd[0]);
                exit(1);
            }
            close(pipefd[0]);    // FD is duplicated, close original

            execute_command_node(connect_cmd->second, envp, &pipefd[0] /* not really original_stdin */, original_stdout);
            exit(g_last_exit_status); // Exit with the status of connect_cmd->second
        }

        // Parent process
        close(pipefd[0]); // Parent closes both ends of the pipe
        close(pipefd[1]);

        waitpid(pid1, &status1, 0); // Wait for first child
        waitpid(pid2, &status2, 0); // Wait for second child

        // The exit status of a pipeline is typically the status of the last command.
        if (WIFEXITED(status2)) {
            g_last_exit_status = WEXITSTATUS(status2);
        } else if (WIFSIGNALED(status2)) {
            g_last_exit_status = 128 + WTERMSIG(status2);
        } else {
            g_last_exit_status = 1; // Should not happen if waited correctly
        }

    } else if (connect_cmd->type == CNT_AND_AND) {
        execute_command_node(connect_cmd->first, envp, original_stdin, original_stdout);
        if (g_last_exit_status == 0) { // If first command succeeded
            execute_command_node(connect_cmd->second, envp, original_stdin, original_stdout);
        }
    } else if (connect_cmd->type == CNT_OR_OR) {
        execute_command_node(connect_cmd->first, envp, original_stdin, original_stdout);
        if (g_last_exit_status != 0) { // If first command failed
            execute_command_node(connect_cmd->second, envp, original_stdin, original_stdout);
        }
    } else if (connect_cmd->type == CNT_NL) { // Sequential execution
        execute_command_node(connect_cmd->first, envp, original_stdin, original_stdout);
        // The status of the first command is g_last_exit_status.
        // Then execute the second command regardless of the first one's status.
        execute_command_node(connect_cmd->second, envp, original_stdin, original_stdout);
        // g_last_exit_status will be that of the second command.
    } else {
        fprintf(stderr, "executor: Unknown connection type %d\n", connect_cmd->type);
        g_last_exit_status = 127;
    }
    return (g_last_exit_status == 0) ? 0 : 1;
}

// --- Subshell Command Executor ---
int execute_subshell_command(t_subshell_cmd *subshell_cmd, t_redirect *cmd_redirects, char **envp) {
    pid_t pid;
    int status;
    int heredoc_temp_fds[2] = {-1, -1};


    pid = fork();
    if (pid == -1) {
        perror("executor: fork for subshell failed");
        g_last_exit_status = 127;
        return 1;
    }

    if (pid == 0) { // Child process (the subshell)
        // Apply redirections for the subshell command ITSELF
        if (handle_redirections(cmd_redirects, NULL, heredoc_temp_fds)) {
            if(heredoc_temp_fds[1] != -1) close(heredoc_temp_fds[1]);
            exit(1); // Redirection error in subshell
        }
        // If a here_doc was used for the subshell's own redirection
        if(heredoc_temp_fds[0] != -1 && (heredoc_temp_fds[0] != STDIN_FILENO /* etc. */)) {
            // close(heredoc_temp_fds[0]); // if duped and no longer needed directly
        }


        // The subshell executes the command within it.
        // The subshell gets its own environment copy implicitly from fork.
        // It doesn't need original_stdin/stdout passed further as it's a new execution context
        // regarding overall shell's stdio. Its stdio is what's set by cmd_redirects.
        execute_command_node(subshell_cmd->command, envp, NULL, NULL); // Pass NULLs or new "originals" for sub-context
        exit(g_last_exit_status); // Subshell exits with the status of its last command
    } else { // Parent process
        if (heredoc_temp_fds[1] != -1) { // Parent closes write end of subshell's heredoc
            close(heredoc_temp_fds[1]);
        }

        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            g_last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            g_last_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT) fprintf(stdout, "\n");
        }
        // No FDs to restore in parent from subshell's own redirections,
        // as those were handled inside the forked subshell.
        if (heredoc_temp_fds[0] != -1) {
             close(heredoc_temp_fds[0]);
        }
    }
    return (g_last_exit_status == 0) ? 0 : 1;
}


// --- Redirection Handler ---
// saved_fds is for parent to restore its FDs. Pass NULL if in child after fork.
// heredoc_fds is an out-parameter: [0] for read_end, [1] for write_end of a *potential* heredoc pipe
// The caller is responsible for closing these pipe ends appropriately.
// `make_here_document` is assumed to have set `r->redirector.fd` to the read end of the here_doc.
int handle_redirections(t_redirect *redirect_list, int *saved_fds, int heredoc_pipe_ends[]) {
    t_redirect *curr = redirect_list;
    int target_fd, source_fd;
    char *filename;

    heredoc_pipe_ends[0] = -1; // Initialize
    heredoc_pipe_ends[1] = -1;

    while (curr) {
        target_fd = -1;
        source_fd = ft_atoi(curr->redirector->word); // FD to be redirected (e.g., 0 for stdin, 1 for stdout)
                                                              // Assumes redirector.filename stores the FD string.
                                                              // Your struct has redirector.fd, use that if populated by parser.
                                                              // Let's use curr->redirector.fd directly if it's already an int.

        // Assuming curr->redirector.fd is the integer FD to be redirected (e.g., 0, 1, 2)
        // And curr->redirectee.filename->word is the target file for >, <, >>
        // Or curr->redirectee.fd is the target FD for >&, <& (not in your current grammar)

        source_fd =ft_atoi(curr->redirector->word); // This should be the FD like 0, 1, or from NUMBER_TOKEN

        // Save the original state of source_fd if saved_fds array is provided (i.e., in parent/before fork)
        if (saved_fds != NULL && source_fd >= 0 && source_fd < 10) { // Simple check for common FDs
            if (saved_fds[source_fd] == -1) { // Save only if not already saved (first redirection for this FD)
                saved_fds[source_fd] = dup(source_fd);
                if (saved_fds[source_fd] == -1 && errno != EBADF) { // EBADF is ok if FD wasn't open
                    perror("executor: dup for saving fd failed");
                    // Potentially restore already changed FDs and error out
                    return 1;
                }
            }
        }

        switch (curr->redir_type) {
            case REDIR_OUTPUT:
                filename = curr->redirectee->word;
                target_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (target_fd == -1) {
                    perror(filename);
                    g_last_exit_status = 1; return 1;
                }
                if (dup2(target_fd, source_fd) == -1) {
                    perror("executor: dup2 REDIR_OUTPUT failed");
                    close(target_fd); g_last_exit_status = 1; return 1;
                }
                close(target_fd); // Original new FD closed after dup2
                break;
            case REDIR_APPEND:
                filename = curr->redirectee->word;
                target_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (target_fd == -1) {
                    perror(filename);
                    g_last_exit_status = 1; return 1;
                }
                if (dup2(target_fd, source_fd) == -1) {
                    perror("executor: dup2 REDIR_APPEND failed");
                    close(target_fd); g_last_exit_status = 1; return 1;
                }
                close(target_fd);
                break;
            case REDIR_INPUT:
                filename = curr->redirectee->word;
                target_fd = open(filename, O_RDONLY);
                if (target_fd == -1) {
                    perror(filename);
                    g_last_exit_status = 1; return 1;
                }
                if (dup2(target_fd, source_fd) == -1) {
                    perror("executor: dup2 REDIR_INPUT failed");
                    close(target_fd); g_last_exit_status = 1; return 1;
                }
                close(target_fd);
                break;
            case REDIR_UNTIL: // Here Document
                // `make_here_document(curr)` is assumed to have:
                // 1. Read the here-doc content.
                // 2. Put it into a temporary pipe or file.
                // 3. Updated `curr->redirector.fd` to be `source_fd` (e.g. 0 for stdin by default)
                // 4. Updated `curr->redirectee.fd` (if using `t_redir union u_redir {int fd; ...}`)
                //    to be the *read end* of the pipe/temp file.
                // Or, more directly, `make_here_document` could populate `heredoc_pipe_ends`.
                // For this example, let's assume make_here_document updated curr->redirectee.fd
                // to be the FD from which to read the here-doc.

                // Let's adjust: assume make_here_document(curr) sets curr->here_doc_fd
                // (a new field in t_redirect for simplicity here) to the read_end of the pipe.
                // And the original parser logic sets curr->redirector.fd to the fd to be redirected (e.g. 0)

                if (ft_atoi(curr->redirector->word) == -1) { // Assuming make_here_document sets this.
                    fprintf(stderr, "executor: here_document not processed correctly\n");
                    g_last_exit_status = 1; return 1;
                }
                if (dup2(ft_atoi(curr->redirector->word), source_fd) == -1) { // source_fd is e.g. STDIN_FILENO
                    perror("executor: dup2 REDIR_UNTIL failed");
                    close(ft_atoi(curr->redirector->word)); // Close the read end we got
                    g_last_exit_status = 1; return 1;
                }
                close(ft_atoi(curr->redirector->word)); // Close the original read end from here_doc setup
                                          // as it's now duplicated to source_fd.
                // The write end of the heredoc pipe should have been closed by make_here_document
                // or by the parent process if it was involved in creating it for a child.
                break;
            default:
                fprintf(stderr, "executor: Unknown redirection type\n");
                g_last_exit_status = 1; return 1;
        }
        curr = curr->next;
    }
    return 0; // Success
}

// --- Helper to build argv for execve ---
char **build_argv(t_word_list *words) {
    int count = 0;
    t_word_list *curr = words;
    while (curr) {
        count++;
        curr = curr->next;
    }
    if (count == 0) return NULL;

    char **argv = (char **)malloc(sizeof(char *) * (count + 1));
    if (!argv) {
        perror("executor: malloc for argv failed");
        return NULL;
    }

    curr = words;
    for (int i = 0; i < count; ++i) {
        // Assuming word->word is the string. Expansion should happen before this.
        argv[i] = strdup(curr->word->word);
        if (!argv[i]) {
            perror("executor: strdup for argv element failed");
            for (int j = 0; j < i; ++j) free(argv[j]);
            free(argv);
            return NULL;
        }
        curr = curr->next;
    }
    argv[count] = NULL; // Null-terminate the array
    return argv;
}

void free_argv(char **argv) {
    if (!argv) return;
    for (int i = 0; argv[i]; ++i) {
        free(argv[i]);
    }
    free(argv);
}


// --- Stubs for functions you need to implement ---
char *resolve_command_path(const char *command) {
    // TODO: Implement PATH searching.
    // - Check if command contains '/'. If so, it's a direct path.
    // - Otherwise, iterate through directories in PATH environment variable.
    // - For each dir, construct full_path = dir + "/" + command.
    // - Check if full_path is executable using access(full_path, X_OK).
    // - Return strdup(full_path) on success, or NULL if not found.
    // Example:
    if (strchr(command, '/')) { // Absolute or relative path
        if (access(command, X_OK) == 0) {
            return strdup(command);
        }
        return NULL;
    }
    // Basic PATH lookup (very simplified)
    char *path_env = getenv("PATH"); // Or search in your envp
    if (!path_env) return NULL;

    char *path_copy = strdup(path_env);
    char *token = strtok(path_copy, ":");
    while (token) {
        char full_path[1024]; // Be careful with buffer overflows
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);
        }
        token = strtok(NULL, ":");
    }
    free(path_copy);
    // If command itself is executable in current dir (if "." is implicitly or explicitly in PATH)
     if (access(command, X_OK) == 0) {
         return strdup(command);
     }
    return NULL;
}

int execute_builtin(char **argv) {
    // TODO: Implement your builtin commands.
    // Compare argv[0] with "cd", "echo", "exit", "export", "unset", "pwd", etc.
    // Execute them directly.
    // `envp_ptr` is char*** if builtins like `export` or `unset` modify the environment.
    // Return:
    //   1: if a builtin was executed successfully.
    //   0: if argv[0] is not a builtin.
    //   >1: if a builtin was executed but failed (specific error code).
    //   -1: for critical builtin error (e.g. exit with bad arg) that should terminate shell
    if (strcmp(argv[0], "exit") == 0) {
        int exit_code = 0;
        if (argv[1]) {
            // TODO: more robust atoi, check for non-numeric
            exit_code = atoi(argv[1]);
        }
        // In a real shell, you'd clean up all resources before exiting.
        printf("exit\n"); // some shells print this
        exit(exit_code); // This exits the whole shell process
        return 255; // Should not be reached
    }
    if (strcmp(argv[0], "cd") == 0) {
        if (!argv[1]) {
            // cd to HOME
            char *home = getenv("HOME"); // or from your envp
            if (home && chdir(home) != 0) perror("cd");
            else if (!home) fprintf(stderr, "cd: HOME not set\n");
        } else {
            if (chdir(argv[1]) != 0) {
                perror(argv[1]); // e.g. "cd: no_such_dir: No such file or directory"
                return 1; // cd error status
            }
        }
        return 1; // cd success status for builtin handler (means success)
    }
    // Add echo, pwd, export, unset ...
    // if (strcmp(argv[0], "echo") == 0) { ... return 1; }


    return 0; // Not a builtin
}
