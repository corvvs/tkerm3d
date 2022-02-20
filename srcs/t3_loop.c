/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:38:39 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 15:38:41 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 表示用変換の構成
void	t3_forming_transform(t_system *system)
{
	const t_optics	*opt = &system->optics;

	t3_affine_scale(system->transform_animated, system->transform_static,
		(t_vector3d){opt->scale_factor, opt->scale_factor, opt->scale_factor});
	t3_affine_rot_axis(system->transform_animated, system->transform_animated,
		opt->rot_axis, opt->phi);
	t3_affine_scale(system->transform_animated, system->transform_animated,
		(t_vector3d){1, -1, 1});
	t3_affine_translate(system->transform_animated, system->transform_animated,
		(t_vector3d){opt->offset_x, opt->offset_y, 0});
}

//　表示変換を適用する
void	t3_transform_to_render(t_system *system)
{
	size_t	i;

	t3_forming_transform(system);
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
	t_ut		t0;
	t_ut		t1;
	t_optics	*opt;
	double		dt;

	t0 = t3_get_ut();
	opt = &system->optics;
	while (true)
	{
		t3_transform_to_render(system);
		t3_render(system);
		t1 = t3_wait_until(t0 + opt->uspf);
		dt = 2 * (0.1 + pow(sin(opt->phi), 0)) * opt->omega * (t1 - t0);
		if (opt->animate)
			opt->phi += dt;
		t0 = t1;
		t3_update_by_key(system);
	}
}
