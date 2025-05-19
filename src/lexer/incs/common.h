/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:52:30 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/19 19:25:13 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMON_H
# define COMMON_H

# include "libft.h"
# include "token.h"

# define DOLLAR 1
# define QUOTED 2
# define SINGLE_QUOTE 4
# define DOUBLE_QUOTE 8
# define ASSIGNMENT 16

t_token_type	get_token_type(char *str);

#endif // COMMON_H