/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_affine_compose.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:43:32 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/23 11:48:55 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// set affine transform `a` to an identity.
void	t3_affine_identity(t_affine a)
{
	int	i;

	i = 0;
	while (i < 4 * 4)
	{
		a[i / 4][i % 4] = (i / 4) == (i % 4);
		i += 1;
	}
}

// copy affine transform `src` to `dest`.
void	t3_affine_copy(t_affine dest, const t_affine src)
{
	int	i;

	if (dest == src)
		return ;
	i = 0;
	while (i < 4 * 4)
	{
		dest[i / 4][i % 4] = src[i / 4][i % 4];
		i += 1;
	}
}

// compose affine transform `src` and a translation, and set it to `dest`.
void	t3_affine_translate(
	t_affine dest, const t_affine src, const t_vector3d vec)
{
	const double	r03 = src[0][3] + vec[0];
	const double	r13 = src[1][3] + vec[1];
	const double	r23 = src[2][3] + vec[2];

	t3_affine_copy(dest, src);
	dest[0][3] = r03;
	dest[1][3] = r13;
	dest[2][3] = r23;
}

// compose affine transform `src` and a scaling, and set it to `dest`.
void	t3_affine_scale(t_affine dest, const t_affine src, const t_vector3d vec)
{
	int	i;
	int	j;

	t3_affine_copy(dest, src);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 4)
		{
			dest[i][j] *= vec[i];
			j += 1;
		}
		i += 1;
	}
}

// compose affine transform `src1` and `src2`, and set it to `dest`.
void	t3_affine_compose(
	t_affine dest, const t_affine src1, const t_affine src2)
{
	t_affine	temp;
	int			i;
	int			j;
	int			k;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			temp[i][j] = 0;
			k = -1;
			while (++k < 4)
				temp[i][j] += src1[i][k] * src2[k][j];
		}
	}
	t3_affine_copy(dest, temp);
}
