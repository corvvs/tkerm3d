/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:38:39 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/23 11:02:42 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 表示用変換を構成する。以下の変換を順に合成:
// 1. ズーム倍率に基づいて拡大
// 2. 現在の回転軸まわりで時間経過による回転
// 3. Y-UPにするためにYのみ反転
// 4. カメラオフセットに基づいて並行移動
static void	forming_transform(t_system *system)
{
	const t_optics	*opt = &system->optics;
	const double	time = t3_fmod(opt->t / 1000000, 1);
	const double	rotation = t3_fmod(
		t3_ratio_by_time(&system->optics.bezier, time), 1);

	t3_affine_scale(system->transform_animated, system->transform_static,
		(t_vector3d){opt->scale_factor, opt->scale_factor, opt->scale_factor});
	t3_affine_rot_axis(system->transform_animated, system->transform_animated,
		opt->rot_axis, 2 * M_PI * rotation);
	t3_affine_scale(system->transform_animated, system->transform_animated,
		(t_vector3d){1, -1, 1});
	t3_affine_translate(system->transform_animated, system->transform_animated,
		(t_vector3d){opt->offset_x, opt->offset_y, 0});
}

//　表示変換を適用する
static void	transform_to_render(t_system *system)
{
	size_t	i;

	forming_transform(system);
	i = 0;
	while (i < system->n_points)
	{
		t3_apply_transform(
			system->points_animated[i],
			system->points_original[i],
			system->transform_animated);
		i += 1;
	}
}

// [描画ループ]
void	t3_render_loop(t_system *system)
{
	t_ut		t_last_frame;
	t_ut		t_curr_frame;
	t_optics	*opt;

	t_last_frame = t3_get_ut();
	opt = &system->optics;
	while (true)
	{
		transform_to_render(system);
		t3_render(system);
		t_curr_frame = t3_wait_until(t_last_frame + opt->us_per_frame);
		if (opt->animate)
			opt->t += (t_curr_frame - t_last_frame) * opt->omega;
		t_last_frame = t_curr_frame;
		t3_update_by_key(system);
	}
}
