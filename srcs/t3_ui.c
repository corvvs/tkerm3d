/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_ui.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 10:37:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 11:10:31 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_repoint(t_system *system)
{
	size_t	size;

	if (system->points_animated)
	{
		free(system->points_animated);
	}
	size = sizeof(t_vector3d) * (system->n_points + 1);
	system->points_animated = malloc(size);
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
int	t3_get_key(void)
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
	if (ch != EOF)
		return (ch);
	return (0);
}

void	t3_update_omega(t_system *system, int key)
{
	if (key == 'e' || key == 'q')
	{
		system->optics.omega += ((key == 'e') * 2 - 1) * 0.000001;
		dprintf(
			STDERR_FILENO,
			"omega -> %f[rot/s]\n",
			system->optics.omega * 1000000 / M_PI / 2
			);
	}
}

void	t3_update_offset(t_system *system, int key)
{
	if (key == 'd' || key == 'a')
	{
		system->optics.offset_x += ((key == 'd') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.x -> %f\n", system->optics.offset_x);
	}
	else if (key == 's' || key == 'w')
	{
		system->optics.offset_y += ((key == 's') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.y -> %f\n", system->optics.offset_x);
	}
}

void	t3_update_flame(t_system *system, int key)
{
	if (key == 'x' || key == 'z')
	{
		if ((key == 'x' && system->optics.fps < 240)
			|| (key == 'z' && 2 < system->optics.fps))
			system->optics.fps += ((key == 'x') * 2 - 1);
		system->optics.uspf = 1000000 / system->optics.fps;
		dprintf(STDERR_FILENO, "fps -> %f\n", system->optics.fps);
	}
}

void	t3_update_scale(t_system *system, int key)
{
	if (key == '1')
	{
		system->optics.scale_factor /= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
	else if (key == '2')
	{
		system->optics.scale_factor *= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
}

void	t3_udpate_axis(t_system *system, int key)
{
	t_vector3d	*axis;

	axis = &system->optics.rot_axis;
	if (key == '3')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){1, 0, 0}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			*axis[0], *axis[1], *axis[2]);
	}
	else if (key == '4')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 1, 0}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			*axis[0], *axis[1], *axis[2]);
	}
	else if (key == '5')
	{
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 0, 1}, M_PI / 1024);
		dprintf(STDERR_FILENO, "rot axis -> (%f, %f, %f)\n",
			*axis[0], *axis[1], *axis[2]);
	}
}

void	t3_update_animate(t_system *system)
{
	system->optics.animate = !system->optics.animate;
	dprintf(STDERR_FILENO, "animate -> %s\n",
		(char *[2]){"NO", "YES"}[!!system->optics.animate]);
}

void	t3_reset_params(t_system *system)
{
	dprintf(STDERR_FILENO, "reset parameters\n");
	t3_init_render_params(system);
}

void	t3_update_by_key(t_system *system)
{
	const int	key = t3_get_key();

	if (!key)
		return ;
	if (strchr("eq", key))
		t3_update_omega(system, key);
	else if (strchr("sadw", key))
		t3_update_offset(system, key);
	else if (strchr("xz", key))
		t3_update_flame(system, key);
	else if (strchr("12", key))
		t3_update_scale(system, key);
	else if (strchr("345", key))
		t3_udpate_axis(system, key);
	else if (key == ' ')
		t3_update_animate(system);
	else if (key == 'r')
		t3_reset_params(system);
	else if (key == T3_CHAR_ESC && system->src_mode == T3_SRC_TEXT)
		t3_scan_message(system);
	else
		dprintf(STDERR_FILENO, "key is %d('%c')\n", key, key);
}
