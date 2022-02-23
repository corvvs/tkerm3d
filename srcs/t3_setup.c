/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_setup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:38:46 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/23 18:06:42 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

static t_cubic_bezier	cubic_bezier(double x1, double y1, double x2, double y2)
{
	t_cubic_bezier	cb;

	cb.x1 = x1;
	cb.y1 = y1;
	cb.x2 = x2;
	cb.y2 = y2;
	return (cb);
}

// stdoutはどちらもターミナルになっている必要がある。
// (まともな出力と、stdinをターミナルに向け直すために必要)
void	t3_check_tty_out(void)
{
	if (isatty(STDOUT_FILENO) == 1)
		return ;
	dprintf(STDERR_FILENO, T3_COLOR_YELLOW
		"Error: stdout shoule be bound to tty."
		T3_COLOR_RESET "\n");
	exit(1);
}

// set all initial parameters of optics.
void	t3_init_render_params(t_system *system)
{
	struct winsize	ws;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	system->optics.animate = true;
	system->optics.width = t3_max(2, t3_min(T3_MAX_WIDTH, ws.ws_col));
	system->optics.height = t3_max(2, t3_min(T3_MAX_HEIGHT, ws.ws_row - 2));
	system->optics.scale_factor = 1;
	system->optics.offset_x = 4;
	system->optics.offset_y = 4;
	system->optics.sq_size_x
		= (double)(2 * system->optics.offset_x) / system->optics.width;
	system->optics.sq_size_y
		= (double)(2 * system->optics.offset_y) / system->optics.height;
	memcpy(system->optics.rot_axis, (t_vector3d){0, 1, 0}, sizeof(double) * 3);
	system->optics.omega = 1.0;
	system->optics.t = 0;
	system->optics.fps = 120;
	system->optics.us_per_frame = 1000000 / system->optics.fps;
	system->n_printbuffer
		= system->optics.height * (system->optics.width + 1);
	system->optics.bezier = cubic_bezier(0.68, -0.6, 0.32, 1.6);
	t3_setup_cubic_bezier(&system->optics.bezier);
}

void	t3_setup_system(t_system *system)
{
	t3_init_render_params(system);
	t3_affine_identity(system->transform_static);
	t3_centralize_points(system->n_points, system->points_original);
	t3_realloc_points(system);
}
