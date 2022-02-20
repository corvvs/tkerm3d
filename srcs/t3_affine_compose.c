/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_affine_compose.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:43:32 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 15:43:34 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// アフィン変換を恒等変換に設定する
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

// アフィン変換をコピーする
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

// vec方向への平行移動とアフィン変換 src を合成したものを dest にセットする
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

// vecによるスケーリングとアフィン変換 src を合成したものを dest にセットする
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

// アフィン変換 src1 と src2 の合成を dest にセットする
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
