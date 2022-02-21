/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph_allocate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:51:21 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/21 22:50:32 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

static void	clear_original_points(t_system *system)
{
	if (system->points_original)
	{
		free(system->points_original);
		system->points_original = NULL;
	}
}

static bool	is_printable(const char c, size_t n_glyphs)
{
	if (' ' <= c && (size_t)c < ' ' + n_glyphs)
		return (true);
	return (false);
}

void	t3_allocate_points(const char *str, t_system *system)
{
	size_t	i;
	size_t	n_points;

	clear_original_points(system);
	i = 0;
	n_points = 0;
	while (str[i])
	{
		if (is_printable(str[i], system->n_glyphs))
			n_points += system->glyphs[str[i] - ' '].n_points;
		i += 1;
	}
	system->n_points = n_points;
	system->points_original = malloc(sizeof(t_vector3d) * (n_points + 1));
	if (!system->points_original)
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: failed to allocate points_original."
			T3_COLOR_RESET "\n");
		exit(1);
	}
}
