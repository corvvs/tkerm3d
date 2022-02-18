/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_ui.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 10:37:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 02:29:47 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_repoint(t_system *system)
{
	if (system->points_animated)
	{
		free(system->points_animated);
	}
	system->points_animated = malloc(sizeof(t_vector3d) * (system->n_points + 1));
}

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
	newt.c_lflag &= ~(ICANON | ECHO | ECHOE);
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

void	t3_update_by_key(t_system *system)
{
	const int key = t3_get_key();

	if (!key)
		return ;
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
			(t_vector3d){1, 0, 0}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == '4')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 1, 0}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == '5')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 0, 1}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			system->optics.rot_axis[0], system->optics.rot_axis[1], system->optics.rot_axis[2]);
	}
	else if (key == ' ')
	{
		system->optics.animate = !system->optics.animate;
		dprintf(STDERR_FILENO, "animate -> %s\n", (char *[2]){"NO","YES"}[!!system->optics.animate]);
	}
	else if (key == 'r')
	{
		dprintf(STDERR_FILENO, "reset parameters\n");
		t3_init_render_params(system);
	}
	else if (key == T3_CHAR_ESC && system->src_mode == T3_SRC_TEXT)
	{
		dprintf(STDERR_FILENO, "key is ESC -> go line mode\n");
		t3_scan_message(system);
	}
	else
		dprintf(STDERR_FILENO, "key is %d('%c')\n", key, key);
}
