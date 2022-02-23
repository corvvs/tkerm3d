/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minilib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 18:00:25 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 18:00:26 by corvvs           ###   ########.fr       */
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

bool	ft_str_ensdwith(const char *str, const char *suffix)
{
	const size_t	nstr = strlen(str);
	const size_t	nsuffix = strlen(suffix);

	if (nstr < nsuffix)
		return (false);
	return (strcmp(str + (nstr - nsuffix), suffix) == 0);
}
