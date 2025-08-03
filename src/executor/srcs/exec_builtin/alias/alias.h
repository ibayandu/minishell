/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_helper.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:47:28 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:26:25 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIAS_H
# define ALIAS_H

# include "builtin.h"
# include "expander.h"

void	list_alias(t_minishell *minishell);
int		ft_alias_with_value(char *arg, t_minishell *minishell);
int		ft_alias_without_value(char *arg, t_minishell *minishell);

#endif // ALIAS_H
