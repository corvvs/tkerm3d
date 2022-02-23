/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_cubic_bezier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 01:01:30 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/22 19:59:54 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

static void	set_points(double v1, double v2, double *vs)
{
	const double	a = 3 * v1 - 3 * v2 + 1;
	const double	b = -6 * v1 + 3 * v2;
	const double	c = 3 * v1;
	size_t			i;
	double			t;

	i = 0;
	t = 0;
	while (i <= T3_CBEZIER_SIZE)
	{
		t = (double)i / T3_CBEZIER_SIZE;
		vs[i] = t * (t * (t * a + b) + c);
		i += 1;
	}
}

static double	interpolate_linearly(t_cubic_bezier *bezier, size_t i, double t)
{
	const double	x1 = bezier->xs[i];
	const double	y1 = bezier->ys[i];
	const double	x2 = bezier->xs[i + 1];
	const double	y2 = bezier->ys[i + 1];

	if (x2 != x1)
		return ((y2 - y1) / (x2 - x1) * (t - x1) + y1);
	else
		return ((y2 + y1) / 2);
}

// 媒介変数による点列 xs(時間), ys(位置) を、
// 線形補完により時間による位置の点列 ps に変換する。
static void	by_time(t_cubic_bezier *bezier)
{
	size_t	i;
	size_t	ti;
	double	t;

	bezier->ps[0] = 0;
	bezier->ps[T3_CBEZIER_SIZE] = 1;
	i = 0;
	ti = 1;
	while (i < T3_CBEZIER_SIZE && ti < T3_CBEZIER_SIZE)
	{
		t = (double)ti / T3_CBEZIER_SIZE;
		while (i < T3_CBEZIER_SIZE && t > bezier->xs[i + 1])
			i += 1;
		while (ti < T3_CBEZIER_SIZE && t <= bezier->xs[i + 1])
		{
			bezier->ps[ti] = interpolate_linearly(bezier, i, t);
			ti += 1;
			t = (double)ti / T3_CBEZIER_SIZE;
		}
	}
}

// cubic-bezier によるアニメーションパラメータの設定
void	t3_setup_cubic_bezier(t_cubic_bezier *bezier)
{
	if (bezier->x1 < 0 || 1 < bezier->x1)
		bezier->x1 = 0.5;
	if (bezier->x2 < 0 || 1 < bezier->x2)
		bezier->x2 = 0.5;
	set_points(bezier->x1, bezier->x2, bezier->xs);
	set_points(bezier->y1, bezier->y2, bezier->ys);
	by_time(bezier);
}

// 時刻 t を、イージングパラメータ ps を使って回転数(rotation) に変換する。
double	t3_ratio_by_time(t_cubic_bezier *bezier, double t)
{
	const size_t	ti = t * T3_CBEZIER_SIZE;
	const double	x1 = (double)ti / T3_CBEZIER_SIZE;
	const double	x2 = (double)(ti + 1) / T3_CBEZIER_SIZE;
	const double	y1 = bezier->ps[ti];
	const double	y2 = bezier->ps[ti + 1];

	return ((y2 - y1) / (x2 - x1) * (t - x1) + y1);
}
