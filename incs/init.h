/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:43:55 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/05 11:50:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

void	initialize_shell_variables(char **env, int *exit_code);
char	*get_prompt(void);

#endif // INIT_H
