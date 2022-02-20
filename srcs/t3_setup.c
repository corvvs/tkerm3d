/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_setup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:38:46 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 15:38:47 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// stdoutはどちらもターミナルになっている必要がある。
// (まともな出力と、stdinをターミナルに向け直すために必要)
void	t3_check_tty_out(void)
{
	if (isatty(STDOUT_FILENO) == 1)
		return ;
	dprintf(STDERR_FILENO, "Error: stdout shoule be bound to tty.\n");
	exit(1);
}

// opticsの設定
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
	system->optics.phi = 0;
	system->optics.omega = M_PI / 1000000;
	system->optics.fps = 120;
	system->optics.uspf = 1000000 / system->optics.fps;
	system->n_pixelbuffer
		= system->optics.height * (system->optics.width + 1);
	if (system->pixelbuffer)
		free(system->pixelbuffer);
	system->pixelbuffer = malloc(sizeof(char) * system->n_pixelbuffer);
}

// [処理用データ構造の初期化]
void	t3_setup_system(t_system *system)
{
	t3_init_render_params(system);
	t3_affine_identity(system->transform_static);
	t3_centralize_points(system->n_points, system->points_original);
	t3_repoint(system);
}
