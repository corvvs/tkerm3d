/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 10:16:48 by corvvs           ###   ########.fr       */
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
	system->optics.omega = M_PI / 1000000;
	// 1000000マイクロ秒(=1秒)をFPSで割ると、1フレームあたりのマイクロ秒数(uspf)が算出される。
	// これを使ってFPSをコントロールする(もっといい方法がありそう)。
	system->optics.fps = 30;
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
			&system->points_animated[i],
			&system->points_original[i],
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
	if (key == 'e')
	{
		system->optics.omega += 0.000001;
		dprintf(STDERR_FILENO, "omega -> %f[rot/s]\n", system->optics.omega * 1000000 / M_PI / 2);
	}
	else if (key == 'q')
	{
		system->optics.omega -= 0.000001;
		dprintf(STDERR_FILENO, "omega -> %f[rot/s]\n", system->optics.omega * 1000000 / M_PI / 2);
	}
	else if (key == 'a')
	{
		system->optics.offset_x -= 0.1;
		dprintf(STDERR_FILENO, "offset.x -> %f\n", system->optics.offset_x);
	}
	else if (key == 'd')
	{
		system->optics.offset_x += 0.1;
		dprintf(STDERR_FILENO, "offset.x -> %f\n", system->optics.offset_x);
	}
	else if (key == 'w')
	{
		system->optics.offset_y -= 0.1;
		dprintf(STDERR_FILENO, "offset.y -> %f\n", system->optics.offset_y);
	}
	else if (key == 's')
	{
		system->optics.offset_y += 0.1;
		dprintf(STDERR_FILENO, "offset.y -> %f\n", system->optics.offset_y);
	}
	else if (key == 'x')
	{
		if (system->optics.fps < 240)
			system->optics.fps += 1;
		system->optics.uspf = 1000000 / system->optics.fps;
		dprintf(STDERR_FILENO, "fps -> %f\n", system->optics.fps);
	}
	else if (key == 'z')
	{
		if (2 < system->optics.fps)
			system->optics.fps -= 1;
		system->optics.uspf = 1000000 / system->optics.fps;
		dprintf(STDERR_FILENO, "fps -> %f\n", system->optics.fps);
	}
}

// [描画ループ]
void	t3_render_loop(t_system *system)
{
	t_ut	t0 = t3_get_ut();
	t_ut	t1;
	int		key_pressed;

	while (true)
	{
		t3_affine_rot_y(system->transform_animated, system->transform_static, system->optics.phi);
		t3_transform_to_render(system);
		t3_clear_pixelbuffer(system);
		t3_fill_pixelbuffer(system);
		t1 = t3_wait_until(t0 + system->optics.uspf);
		t3_render_pixel_buffer(system);
		// powとかsinがある係数は、回転速度をいい感じに揺らすためにかけている
		system->optics.phi += 2 * (0.1 + pow(sin(system->optics.phi), 4)) * system->optics.omega * (t1 - t0);
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
	while (rd_is_finite(system.points_original[system.n_points].x))
	{
		system.n_points += 1;
	}
	t3_setup_system(&system);
	t3_render_loop(&system);
}
