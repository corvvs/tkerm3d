/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph_allocate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:51:21 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 15:51:22 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// STDIN_FILENO を /dev/tty/stdin に割り当て直す
int	t3_stdin_to_tty(void)
{
	close(STDIN_FILENO);
	return (dup2(STDOUT_FILENO, STDIN_FILENO));
}

void	t3_clear_original_points(t_system *system)
{
	if (system->points_original)
	{
		free(system->points_original);
		system->points_original = NULL;
	}
}

bool	t3_is_objectable(const char c, size_t n_glyphs)
{
	if (' ' <= c && (size_t)c < ' ' + n_glyphs)
		return (true);
	return (false);
}

void	t3_allocate_points(const char *str, t_system *system)
{
	size_t	i;
	size_t	n_points;

	t3_clear_original_points(system);
	i = 0;
	n_points = 0;
	while (str[i])
	{
		if (t3_is_objectable(str[i], system->n_glyphs))
			n_points += system->glyphs[str[i] - ' '].n_points;
		i += 1;
	}
	system->n_points = n_points;
	system->points_original = malloc(sizeof(t_vector3d) * (n_points + 1));
}
