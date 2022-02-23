/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_source.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 17:54:52 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 18:29:58 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// bind STDIN_FILENO to tty(for key-control).
static int	stdin_to_tty(void)
{
	close(STDIN_FILENO);
	return (dup2(STDOUT_FILENO, STDIN_FILENO));
}

static void	hande_text(t_system *system)
{
	system->src_mode = T3_SRC_TEXT;
	if (!t3_read_glyphs(system->glyphs))
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: %s: failed to read glyth file."
			T3_COLOR_RESET "\n", T3_GLYPH_FILE);
		exit(1);
	}
	t3_scan_message(system);
	stdin_to_tty();
}

static void	handle_file_3d(t_system *system, const char *filename)
{
	system->src_mode = T3_SRC_FILE_3D;
	system->points_original = t3_read_vectors_from_file(filename);
	if (!system->points_original)
		exit(1);
	system->n_points = 0;
	while (isfinite(system->points_original[system->n_points][0]))
		system->n_points += 1;
}

static t_source	detect_source_type(int argc, char **argv)
{
	const char	*filepath = argv[1];

	if (argc > 2)
		return (T3_SRC_TOO_MANY);
	if (argc == 1)
		return (T3_SRC_TEXT);
	if (argc == 2 && strlen(filepath) > strlen(T3_EXTENSION_3D)
		&& ft_str_endswith(filepath, T3_EXTENSION_3D))
		return (T3_SRC_FILE_3D);
	return (T3_SRC_FILE_UNEXPECTED);
}

void	t3_read_source(t_system *system, int argc, char **argv)
{
	const char	*filepath = argv[1];

	system->src_mode = detect_source_type(argc, argv);
	if (system->src_mode == T3_SRC_TEXT)
		hande_text(system);
	else if (system->src_mode == T3_SRC_FILE_3D)
		handle_file_3d(system, filepath);
	else
	{
		if (system->src_mode == T3_SRC_TOO_MANY)
		{
			dprintf(STDERR_FILENO, T3_COLOR_YELLOW
				"Error: too mamy argument."
				T3_COLOR_RESET "\n");
		}
		else if (system->src_mode == T3_SRC_FILE_UNEXPECTED)
		{
			dprintf(STDERR_FILENO, T3_COLOR_YELLOW
				"Error: %s: we expect .3d extension only."
				T3_COLOR_RESET "\n", filepath);
		}
		exit(1);
	}
}
