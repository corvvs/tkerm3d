/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cntchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:28:18 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 02:29:31 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// count occurences of a character `c` in a string `str`
size_t	ft_cntchr(const char *str, char c)
{
	size_t	n;

	n = 0;
	while (*str)
	{
		n += (*str == c);
		++str;
	}
	return (n);
}
