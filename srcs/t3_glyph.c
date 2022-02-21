/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:01:55 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 15:22:44 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

static size_t	count_points_of_glyph(char **lines, size_t start, size_t end)
{
	size_t	row_index;
	size_t	col_index;
	size_t	n;

	row_index = start;
	n = 0;
	while (row_index < end)
	{
		col_index = 0;
		while (lines[row_index][col_index])
		{
			n += lines[row_index][col_index] == '#';
			col_index += 1;
		}
		row_index += 1;
	}
	return (n);
}

static size_t	next_start_glyph_line(char **lines, size_t index)
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

static bool	convert_glyph_to_points(t_glyph *glyph,
			char **lines, size_t start, size_t end)
{
	size_t	row_index;
	size_t	col_index;
	size_t	dots;

	glyph->n_points = count_points_of_glyph(lines, start, end);
	glyph->points = malloc(sizeof(t_vector3d) * glyph->n_points);
	if (!glyph->points)
		return (false);
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
	return (true);
}

void	validate_glyph_file(char ***lines_ptr)
{
	char	**lines;

	lines = *lines_ptr;
	while (*lines)
	{
		if (strlen(*lines) != T3_GLYPH_WIDTH)
		{
			t3_destroy_strarray(*lines_ptr);
			dprintf(STDERR_FILENO, "Error: %s: invalid file.\n", T3_GLYPH_FILE);
			exit(1);
		}
		lines += 1;
	}
	if (lines - *lines_ptr != T3_GLYPH_HEIGHT * T3_GLYPH_NUM)
	{
		t3_destroy_strarray(*lines_ptr);
		dprintf(STDERR_FILENO, "Error: %s: invalid file.\n", T3_GLYPH_FILE);
		exit(1);
	}
}

// グリフファイルからグリフを読み取り、t_glyph配列に格納する。
// 読み取れたグリフの数を返す(最大でT3_GLYPH_NUM)。
size_t	t3_read_glyphs(t_glyph *glyphs)
{
	char	**lines;
	size_t	i;
	size_t	next_start_line;
	size_t	curr_start_line;

	lines = t3_read_all_lines(T3_GLYPH_FILE);
	if (!lines)
		return (0);
	validate_glyph_file(&lines);
	i = 0;
	next_start_line = 0;
	curr_start_line = 0;
	while (i < T3_GLYPH_NUM)
	{
		next_start_line = next_start_glyph_line(lines, next_start_line);
		if (curr_start_line == next_start_line)
			break ;
		convert_glyph_to_points(
			&glyphs[i], lines, curr_start_line, next_start_line);
		curr_start_line = next_start_line;
		i += 1;
	}
	t3_destroy_strarray(lines);
	dprintf(STDERR_FILENO, "i = %zu\n", i);
	return (i);
}
