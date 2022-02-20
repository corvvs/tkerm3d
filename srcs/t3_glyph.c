/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:01:55 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 18:16:32 by corvvs           ###   ########.fr       */
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

char	**t3_glyph_array(const char *filename)
{
	char	*content;
	char	**lines;

	content = rd_read_file_content(filename);
	if (!content)
		return (NULL);
	lines = ft_split(content, '\n');
	free(content);
	return (lines);
}

size_t	t3_count_glyph_dots(char **lines, size_t start, size_t end)
{
	size_t	row_index;
	size_t	col_index;
	size_t	dots;

	row_index = start;
	dots = 0;
	while (row_index < end)
	{
		col_index = 0;
		while (lines[row_index][col_index])
		{
			dots += lines[row_index][col_index] == '#';
			col_index += 1;
		}
		row_index += 1;
	}
	return (dots);
}

size_t	t3_next_glyph(char **lines, size_t index)
{
	size_t	i;

	i = index;
	while (lines[i])
	{
		i += 1;
		if (i % T3_GLYPH_HEIGHT == 0)
			break ;
	}
	return (i);
}

void	t3_read_glyph_dots(t_glyph *glyph,
			char **lines, size_t start, size_t end)
{
	size_t	row_index;
	size_t	col_index;
	size_t	dots;

	glyph->n_points = t3_count_glyph_dots(lines, start, end);
	glyph->points = malloc(sizeof(t_vector3d) * glyph->n_points);
	row_index = start;
	dots = 0;
	while (row_index < end)
	{
		col_index = 0;
		while (lines[row_index][col_index])
		{
			if (lines[row_index][col_index] == '#')
			{
				glyph->points[dots][0] = col_index * T3_GLYPH_SCALE;
				glyph->points[dots][1] = (end - row_index) * T3_GLYPH_SCALE;
				glyph->points[dots][2] = 0;
				dots += 1;
			}
			col_index += 1;
		}
		row_index += 1;
	}
}

// グリフファイルからグリフを読み取り、t_glyph配列に格納する。
// 読み取れたグリフの数を返す(最大でT3_GLYPH_NUM)。
size_t	t3_read_glyph(t_glyph *glyphs)
{
	char	**lines;
	size_t	g;
	size_t	next_glyph;
	size_t	curr_glyph;

	lines = t3_glyph_array("./printables.txt");
	if (!lines)
		return (0);
	g = 0;
	next_glyph = 0;
	curr_glyph = 0;
	while (g != T3_GLYPH_NUM)
	{
		next_glyph = t3_next_glyph(lines, next_glyph);
		t3_read_glyph_dots(&glyphs[g], lines, curr_glyph, next_glyph);
		curr_glyph = next_glyph;
		g += 1;
	}
	rd_free_strarray(&lines);
	return (g);
}

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
