/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:01:55 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 11:59:08 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

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
	t3_destroy_strarray(lines);
	return (g);
}
