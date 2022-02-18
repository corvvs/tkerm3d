/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 00:47:38 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// opticsの設定
void	t3_init_optics(t_system *system)
{
	struct winsize	ws;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	system->optics.animate = true;
	system->optics.width = ws.ws_col < T3_MAX_WIDTH ? ws.ws_col : T3_MAX_WIDTH;
	system->optics.height = ws.ws_row < T3_MAX_HEIGHT ? ws.ws_row : T3_MAX_HEIGHT;
	system->optics.scale_factor = 1;
	system->optics.offset_x = 4;
	system->optics.offset_y = 4;
	system->optics.sq_size_x = (double)(2 * system->optics.offset_x) / system->optics.width;
	system->optics.sq_size_y = (double)(2 * system->optics.offset_y) / system->optics.height;
	memcpy(system->optics.rot_axis, (t_vector3d){0, 1, 0}, sizeof(double) * 3);
	system->optics.phi = 0;
	system->optics.omega = M_PI / 1000000;
	system->optics.fps = 120;
	system->optics.uspf = 1000000 / system->optics.fps;
}

// [処理用データ構造の初期化]
void	t3_setup_system(t_system *system)
{
	t3_init_optics(system);
	t3_affine_identity(system->transform_static);
	t3_centralize_points(system->n_points, system->points_original);
	t3_repoint(system);
}

//　表示変換を適用する
void	t3_transform_to_render(t_system *system)
{
	size_t	i;

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

// [描画ループ]
void	t3_render_loop(t_system *system)
{
	t_ut		t0;
	t_ut		t1;
	t_optics	*opt;

	t0 = t3_get_ut();
	opt = &system->optics;
	while (true)
	{
		t3_forming_transform(system);
		t3_transform_to_render(system);
		t3_clear_pixelbuffer(system);
		t3_fill_pixelbuffer(system);
		t1 = t3_wait_until(t0 + opt->uspf);
		t3_render_pixel_buffer(system);
		if (opt->animate)
			opt->phi += 2 * (0.1 + pow(sin(opt->phi), 0)) * opt->omega * (t1 - t0);
		t0 = t1;
		t3_update_by_key(system);
	}
}

int main(int argc, char **argv)
{
	t_system	system;

	if (argc > 2)
	{
		exit(1);
	}
	bzero(&system, sizeof(t_system));
	if (argc == 1)
	{
		system.src_mode = T3_SRC_TEXT;
		system.n_glyphs = t3_read_glyph(system.glyphs);
		printf("input a string you wanna spin:\n");
		t3_scan_message(&system);
		t3_rebind_stdin();
	}
	else
	{
		system.src_mode = T3_SRC_FILE_3D;
		system.points_original = t3_read_vectors_from_file(argv[1]);
		if (!system.points_original)
		{
			exit(1);
		}
		system.n_points = 0;
		while (t3_is_fintie(system.points_original[system.n_points][0]))
		{
			system.n_points += 1;
		}
	}
	t3_setup_system(&system);
	t3_render_loop(&system);
}
