/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_scan_message.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:55:00 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 15:55:01 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	eval_string(char *str)
{
	size_t	i;
	size_t	k;
	bool	escaped;

	i = 0;
	k = 0;
	escaped = false;
	while (str[i])
	{
		if (!escaped && str[i] == '\\')
			escaped = true;
		else
		{
			if (escaped)
			{
				if (str[i] == 'n')
					str[i] = '\n';
			}
			str[k++] = str[i];
			escaped = false;
		}
		i += 1;
	}
	while (k < i)
		str[k++] = '\0';
}

size_t	t3_place_character(t_system *system,
			const char c, size_t n_points, t_position pos)
{
	size_t	j;

	memcpy(system->points_original + n_points,
		system->glyphs[c - ' '].points,
		sizeof(t_vector3d) * system->glyphs[c - ' '].n_points
		);
	j = 0;
	while (j < system->glyphs[c - ' '].n_points)
	{
		system->points_original[n_points + j][0]
			+= T3_GLYPH_SCALE * T3_GLYPH_WIDTH * pos[1];
		system->points_original[n_points + j][1]
			-= T3_GLYPH_SCALE * T3_GLYPH_HEIGHT * pos[0];
		j += 1;
	}
	n_points += system->glyphs[c - ' '].n_points;
	return (n_points);
}

void	t3_str_to_points(t_system *system, const char *str)
{
	size_t		i;
	t_position	pos;
	size_t		n_points;

	t3_allocate_points(str, system);
	n_points = 0;
	i = 0;
	pos[0] = 0;
	pos[1] = 0;
	while (str[i])
	{
		if (isprint(str[i]))
		{
			n_points = t3_place_character(system, str[i], n_points, pos);
			pos[1] += 1;
		}
		else if (str[i] == '\n')
		{
			pos[0] += 1;
			pos[1] = 0;
		}
		i += 1;
	}
	t3_centralize_points(system->n_points, system->points_original);
}

bool	t3_scan_message(t_system *sys)
{
	ssize_t	read_len;

	dprintf(STDERR_FILENO, "input a string you wanna spin:\n");
	bzero(sys->message, sizeof(char) * (T3_MAX_MSGLEN + 1));
	read_len = read(STDIN_FILENO, sys->message, T3_MAX_MSGLEN);
	if (read_len >= 0)
	{
		eval_string(sys->message);
		t3_str_to_points(sys, sys->message);
		t3_repoint(sys);
	}
	return (true);
}
