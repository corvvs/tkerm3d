/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_scan_message.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:55:00 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/21 12:14:38 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// バックスラッシュによる改行文字のエスケープを破壊的に行う
static void	escape_message(char *str)
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

// 文字 c の点群データを points_original に追加し、追加後の points_original の要素数を返す。
static size_t	t3_place_character(t_system *system,
			const char c, size_t n_points, t_position pos)
{
	size_t		j;
	const int	index = c - ' ';
	t_vector3d	*points;

	points = system->points_original;
	memcpy(points + n_points,
		system->glyphs[index].points,
		sizeof(t_vector3d) * system->glyphs[index].n_points);
	j = 0;
	while (j < system->glyphs[index].n_points)
	{
		points[n_points + j][0] += T3_GLYPH_SCALE * T3_GLYPH_WIDTH * pos[1];
		points[n_points + j][1] -= T3_GLYPH_SCALE * T3_GLYPH_HEIGHT * pos[0];
		j += 1;
	}
	n_points += system->glyphs[index].n_points;
	return (n_points);
}

// ユーザが入力したメッセージ str を点群データに変換して取り込む
static void	message_to_points(t_system *system, const char *message)
{
	size_t		i;
	t_position	pos;
	size_t		n_points;

	t3_allocate_points(message, system);
	n_points = 0;
	i = 0;
	pos[0] = 0;
	pos[1] = 0;
	while (message[i])
	{
		if (isprint(message[i]))
		{
			n_points = t3_place_character(system, message[i], n_points, pos);
			pos[1] += 1;
		}
		else if (message[i] == '\n')
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

	if (isatty(STDIN_FILENO))
		dprintf(STDERR_FILENO, "input a string you wanna spin:\n");
	bzero(sys->message, sizeof(char) * (T3_MAX_MSGLEN + 1));
	read_len = read(STDIN_FILENO, sys->message, T3_MAX_MSGLEN);
	if (read_len >= 0)
	{
		escape_message(sys->message);
		message_to_points(sys, sys->message);
		t3_repoint(sys);
	}
	dd_malloc_balance(0);
	return (true);
}
