/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 18:57:51 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// n個の位置ベクトルを、重心が(0, 0, 0)になるように平行移動する。
void	t3_centralize_points(size_t n, t_vector3d *points)
{
	size_t		i;
	t_vector3d	c;

	bzero(&c, sizeof(double) * 3);
	i = 0;
	while (i < n)
	{
		c[0] += points[i][0];
		c[1] += points[i][1];
		c[2] += points[i][2];
		i += 1;
	}
	c[0] /= n;
	c[1] /= n;
	c[2] /= n;
	i = 0;
	while (i < n)
	{
		points[i][0] -= c[0];
		points[i][1] -= c[1];
		points[i][2] -= c[2];
		i += 1;
	}
}

// opticsの設定
void	t3_init_optics(t_system *system)
{
	system->optics.animate = true;
	system->optics.width = T3_WIDTH;
	system->optics.height = T3_HEIGHT;
	system->optics.scale_factor = 1;
	system->optics.offset_x = 4;
	system->optics.offset_y = 4;
	system->optics.sq_size_x = (double)(2 * system->optics.offset_x) / T3_WIDTH;
	system->optics.sq_size_y = (double)(2 * system->optics.offset_y) / T3_HEIGHT;
	memcpy(system->optics.rot_axis, (t_vector3d){0, 1, 0}, sizeof(double) * 3);
	system->optics.phi = 0;
	// 2秒で半周するように角速度を設定
	system->optics.omega = M_PI / 1000000;
	// 1000000マイクロ秒(=1秒)をFPSで割ると、1フレームあたりのマイクロ秒数(uspf)が算出される。
	// これを使ってFPSをコントロールする(もっといい方法がありそう)。
	system->optics.fps = 120;
	system->optics.uspf = 1000000 / system->optics.fps;
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
			system->points_animated[i],
			system->points_original[i],
			system->transform_animated);
		i += 1;
	}
}

// キーが押されていたら押されたキーを返す
// そうでなければ0を返す
int t3_get_key(void)
{
	struct termios	oldt;
	struct termios	newt;
	int				ch;
	int				oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF) {
		return ch;
	}
	return 0;
}

void	t3_update_by_key(t_system *system, int key)
{
	if (key == 'e' || key == 'q')
	{
		system->optics.omega += ((key == 'e') * 2 - 1) * 0.000001;
		dprintf(STDERR_FILENO, "omega -> %f[rot/s]\n", system->optics.omega * 1000000 / M_PI / 2);
	}
	else if (key == 'd' || key == 'a')
	{
		system->optics.offset_x += ((key == 'd') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.x -> %f\n", system->optics.offset_x);
	}
	else if (key == 's' || key == 'w')
	{
		system->optics.offset_y += ((key == 'w') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.y -> %f\n", system->optics.offset_x);
	}
	else if (key == 'x' || key == 'z')
	{
		if ((key == 'x' && system->optics.fps < 240) ||
			(key == 'z' && 2 < system->optics.fps))
			system->optics.fps += ((key == 'x') * 2 - 1);
		system->optics.uspf = 1000000 / system->optics.fps;
		dprintf(STDERR_FILENO, "fps -> %f\n", system->optics.fps);
	}
	else if (key == '1')
	{
		system->optics.scale_factor /= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
	else if (key == '2')
	{
		system->optics.scale_factor *= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
	else if (key == '3')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){1, 0, 0}, M_PI / 100);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == '4')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 1, 0}, M_PI / 100);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == '5')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 0, 1}, M_PI / 100);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == ' ')
	{
		system->optics.animate = !system->optics.animate;
		dprintf(STDERR_FILENO, "animate -> %s\n", (char *[2]){"NO","YES"}[!!system->optics.animate]);
	}
}

// [描画ループ]
void	t3_render_loop(t_system *system)
{
	t_ut		t0 = t3_get_ut();
	t_ut		t1;
	int			key_pressed;
	t_optics	*opt;

	opt = &system->optics;
	while (true)
	{
		t3_affine_scale(system->transform_animated, system->transform_static,
			(t_vector3d){opt->scale_factor, opt->scale_factor, opt->scale_factor});
		t3_affine_rot_axis(system->transform_animated, system->transform_animated, opt->rot_axis, opt->phi);
		t3_affine_scale(system->transform_animated, system->transform_animated,
			(t_vector3d){1, -1, 1});
		t3_affine_translate(system->transform_animated, system->transform_animated,
			(t_vector3d){opt->offset_x, opt->offset_y, 0});
		t3_transform_to_render(system);
		t3_clear_pixelbuffer(system);
		t3_fill_pixelbuffer(system);
		t1 = t3_wait_until(t0 + opt->uspf);
		t3_render_pixel_buffer(system);
		// powとかsinがある係数は、回転速度をいい感じに揺らすためにかけている
		if (opt->animate)
			opt->phi += 2 * (0.1 + pow(sin(opt->phi), 0)) * opt->omega * (t1 - t0);
		t0 = t1;
		key_pressed = t3_get_key();
		if (key_pressed)
		{
			t3_update_by_key(system, key_pressed);
		}
	}
}

int main(int argc, char **argv)
{
	t_system	system;

	setvbuf(stdout, NULL, _IONBF, 0);
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
	while (rd_is_finite(system.points_original[system.n_points][0]))
	{
		system.n_points += 1;
	}
	t3_setup_system(&system);
	t3_render_loop(&system);
}
