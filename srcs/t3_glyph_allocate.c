/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph_allocate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:51:21 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/23 11:25:05 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// ユーザの入力文字列 message に対する点群情報をアロケートする
// allocate points of a given message
void	t3_allocate_points_for_message(const char *message, t_system *system)
{
	size_t	i;
	size_t	n_points;

	free(system->points_original);
	system->points_original = NULL;
	i = 0;
	n_points = 0;
	while (message[i])
	{
		n_points += system->glyphs[message[i] - ' '].n_points;
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
