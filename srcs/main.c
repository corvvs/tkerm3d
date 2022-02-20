/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 18:16:15 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_handle_text(t_system *system)
{
	system->src_mode = T3_SRC_TEXT;
	system->n_glyphs = t3_read_glyph(system->glyphs);
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
	while (t3_is_fintie(system->points_original[system->n_points][0]))
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
