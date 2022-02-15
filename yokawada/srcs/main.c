/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/15 22:48:51 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// n個の位置ベクトルを、重心が(0, 0, 0)になるように平行移動する。
void	t3_centralize_points(size_t n, t_vector3d *points)
{
	size_t		i;
	t_vector3d	c;

	bzero(&c, sizeof(t_vector3d));
	i = 0;
	while (i < n)
	{
		c.x += points[i].x;
		c.y += points[i].y;
		c.z += points[i].z;
		i += 1;
	}
	c.x /= n;
	c.y /= n;
	c.z /= n;
	i = 0;
	while (i < n)
	{
		points[i].x -= c.x;
		points[i].y -= c.y;
		points[i].z -= c.z;
		i += 1;
	}
}

// opticsの設定
void	t3_init_optics(t_system *system)
{
	system->optics.width = T3_WIDTH;
	system->optics.height = T3_HEIGHT;
	system->optics.offset_x = 4;
	system->optics.offset_y = 4;
	system->optics.sq_size_x = (double)(+4 - -4) / T3_WIDTH;
	system->optics.sq_size_y = (double)(+4 - -4) / T3_HEIGHT;
	system->optics.phi = 0;
	// 2秒で半周するように角速度を設定
	system->optics.omega = M_PI * 2 / 1000000;
	// 1000000マイクロ秒(=1秒)をFPSで割ると、1フレームあたりのマイクロ秒数(uspf)が算出される。
	// これを使ってFPSをコントロールする(もっといい方法がありそう)。
	system->optics.uspf = 1000000 / T3_FPS;
}

// [処理用データ構造の初期化]
void	t3_setup_system(t_system *system)
{
	t3_init_optics(system);
	system->points_animated = malloc(sizeof(t_vector3d) * (system->n_points + 1));
	t3_centralize_points(system->n_points, system->points_original);
	t3_affine_identity(system->transform_static);
}

//　表示変換を適用する
void	t3_transform_to_render(t_system *system)
{
	size_t	i;

	i = 0;
	while (i < system->n_points)
	{
		t3_apply_transform(
			&system->points_animated[i],
			&system->points_original[i],
			system->transform_animated);
		i += 1;
	}
}

// [描画ループ]
void	t3_render_loop(t_system *system)
{
	t_ut	t0 = t3_get_ut();
	t_ut	t1;
	while (true)
	{
		t3_affine_rot_y(system->transform_animated, system->transform_static, system->optics.phi);
		t3_transform_to_render(system);
		t3_clear_pixelbuffer(system);
		t3_fill_pixelbuffer(system);
		t1 = t3_wait_until(t0 + system->optics.uspf);
		t3_render_pixel_buffer(system);
		// powとかsinがある係数は、回転速度をいい感じに揺らすためにかけている
		system->optics.phi += (0.1 + pow(sin(system->optics.phi), 4)) * system->optics.omega * (t1 - t0);
		t0 = t1;
	}
}

int main(int argc, char **argv)
{
	t_system	system;

	if (argc != 2)
	{
		exit(1);
	}
	// [ファイルからのデータ読み取り]
	system.points_original = t3_read_from_file(argv[1]);
	if (!system.points_original)
	{
		exit(1);
	}
	system.n_points = 0;
	while (rd_is_finite(system.points_original[system.n_points].x))
	{
		system.n_points += 1;
	}
	t3_setup_system(&system);
	t3_render_loop(&system);
}
