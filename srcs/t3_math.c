/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 01:56:47 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 11:50:45 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

int	t3_max(int a, int b)
{
	if (a < b)
		return (b);
	return (a);
}

int	t3_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

// wrapper for fmod(double, double).
// it behaves more *arithmetically* for negative x.
double	t3_fmod(double x, double y)
{
	if (x >= 0)
		return (fmod(x, y));
	return (y - fmod(-x, y));
}
