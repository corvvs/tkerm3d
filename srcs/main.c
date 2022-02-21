/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 21:13:54 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 各グリフの点群情報を破壊
// (グリフ配列は固定長)
static void	destroy_glyphs(t_system *system)
{
	size_t	i;

	i = 0;
	while (i < system->n_glyphs)
	{
		free(system->glyphs[i].points);
		i += 1;
	}
}

void	t3_handle_text(t_system *system)
{
	system->src_mode = T3_SRC_TEXT;
	system->n_glyphs = t3_read_glyphs(system->glyphs);
	if (system->n_glyphs != T3_GLYPH_NUM)
	{
		destroy_glyphs(system);
		system->n_glyphs = 0;
	}
	if (system->n_glyphs == 0)
		exit(1);
	t3_scan_message(system);
	t3_stdin_to_tty();
}

void	t3_handle_file(t_system *system, char *filename)
{
	system->src_mode = T3_SRC_FILE_3D;
	system->points_original = t3_read_vectors_from_file(filename);
	if (!system->points_original)
		exit(1);
	system->n_points = 0;
	while (isfinite(system->points_original[system->n_points][0]))
		system->n_points += 1;
}

int	main(int argc, char **argv)
{
	t_system	system;

	if (argc > 2)
		exit(1);
	t3_check_tty_out();
	bzero(&system, sizeof(t_system));
	if (argc == 1)
		t3_handle_text(&system);
	else
		t3_handle_file(&system, argv[1]);
	t3_setup_system(&system);
	t3_render_loop(&system);
}
