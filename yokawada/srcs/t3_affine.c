/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_affine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 21:40:43 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 11:11:55 by corvvs           ###   ########.fr       */
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
void	t3_affine_translate(t_affine dest, const t_affine src, const t_vector3d vec)
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

// X軸まわりの回転(角度phi)とアフィン変換 src を合成したものを dest にセットする
void	t3_affine_rot_x(t_affine dest, const t_affine src, double phi)
{
	const double	r11 = +cos(phi) * src[1][1] - sin(phi) * src[2][1];
	const double	r12 = +cos(phi) * src[1][2] - sin(phi) * src[2][2];
	const double	r21 = +sin(phi) * src[1][1] + cos(phi) * src[2][1];
	const double	r22 = +sin(phi) * src[1][2] + cos(phi) * src[2][2];

	t3_affine_copy(dest, src);
	dest[1][1] = r11;
	dest[1][2] = r12;
	dest[2][1] = r21;
	dest[2][2] = r22;
}

// Y軸まわりの回転(角度phi)とアフィン変換 src を合成したものを dest にセットする
void	t3_affine_rot_y(t_affine dest, const t_affine src, double phi)
{
	
	const double	r00 = +cos(phi) * src[0][0] - sin(phi) * src[2][0];
	const double	r02 = +cos(phi) * src[0][2] - sin(phi) * src[2][2];
	const double	r20 = +sin(phi) * src[0][0] + cos(phi) * src[2][0];
	const double	r22 = +sin(phi) * src[0][2] + cos(phi) * src[2][2];

	t3_affine_copy(dest, src);
	dest[0][0] = r00;
	dest[0][2] = r02;
	dest[2][0] = r20;
	dest[2][2] = r22;
}

// ベクトルv1に変換trを適用した結果をベクトルv2に格納する
// v1 と v2は同一であってはならない
void	t3_apply_transform(t_vector3d v2, const t_vector3d v1, t_affine a)
{
	v2[0] = a[0][0] * v1[0] + a[0][1] * v1[1] + a[0][2] * v1[2] + a[0][3];
	v2[1] = a[1][0] * v1[0] + a[1][1] * v1[1] + a[1][2] * v1[2] + a[1][3];
	v2[2] = a[2][0] * v1[0] + a[2][1] * v1[1] + a[2][2] * v1[2] + a[2][3];
}
