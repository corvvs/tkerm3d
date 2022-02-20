/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_affine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 21:40:43 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 00:24:46 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// axis まわりの回転(角度phi)とアフィン変換 src を合成したものを dest にセットする
// || axis || == 1 を仮定する。
void	t3_affine_rot_axis(
	t_affine dest, const t_affine src, const t_vector3d n, double phi)
{
	t_affine		rot;
	const double	c = cos(phi);
	const double	s = sin(phi);
	const double	rc = 1 - c;

	memcpy(rot[0], (double []){
		c + n[0] * n[0] * rc,
		n[0] * n[1] * rc - n[2] * s,
		n[2] * n[0] * rc + n[1] * s, 0}, sizeof(double) * 4);
	memcpy(rot[1], (double []){
		n[0] * n[1] * rc + n[2] * s,
		c + n[1] * n[1] * rc,
		n[1] * n[2] * rc - n[0] * s, 0}, sizeof(double) * 4);
	memcpy(rot[2], (double []){
		n[2] * n[0] * rc - n[1] * s,
		n[1] * n[2] * rc + n[0] * s,
		c + n[2] * n[2] * rc, 0}, sizeof(double) * 4);
	memcpy(rot[3], (double []){0, 0, 0, 1}, sizeof(double) * 4);
	t3_affine_compose(dest, rot, src);
}

// ベクトルv1に変換trを適用した結果をベクトルv2に格納する
// v1 と v2は同一であってはならない
void	t3_apply_transform(t_vector3d v2, const t_vector3d v1, t_affine a)
{
	v2[0] = a[0][0] * v1[0] + a[0][1] * v1[1] + a[0][2] * v1[2] + a[0][3];
	v2[1] = a[1][0] * v1[0] + a[1][1] * v1[1] + a[1][2] * v1[2] + a[1][3];
	v2[2] = a[2][0] * v1[0] + a[2][1] * v1[1] + a[2][2] * v1[2] + a[2][3];
}

// 単位ベクトル r を 回転軸 n のまわりに phi だけ回す
void	t3_rot_vector_around_axis(t_vector3d r, t_vector3d n, double phi)
{
	const double	c = cos(phi);
	const double	s = sin(phi);
	double			t;
	t_vector3d		u;

	t = n[0] * n[0] * r[0];
	u[0] = (r[0] - t) * c + t + (n[1] * r[2] - n[2] * r[1]) * s;
	t = n[1] * n[1] * r[1];
	u[1] = (r[1] - t) * c + t + (n[2] * r[0] - n[0] * r[2]) * s;
	t = n[2] * n[2] * r[2];
	u[2] = (r[2] - t) * c + t + (n[0] * r[1] - n[1] * r[0]) * s;
	memcpy(r, u, sizeof(double) * 3);
}
