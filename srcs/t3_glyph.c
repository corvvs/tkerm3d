/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_glyph.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:01:55 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 10:56:59 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// グリフファイルの「形状」が正しいことを確認する。
static void	validate_shape_of_glyph_file(char ***lines_ptr)
{
	char	**lines;

	lines = *lines_ptr;
	while (*lines)
	{
		if (strlen(*lines) != T3_GLYPH_WIDTH)
		{
			t3_destroy_strarray(*lines_ptr);
			dprintf(STDERR_FILENO, T3_COLOR_YELLOW
				"Error: %s: invalid file."
				T3_COLOR_RESET "\n", T3_GLYPH_FILE);
			exit(1);
		}
		lines += 1;
	}
	if (lines - *lines_ptr != T3_GLYPH_HEIGHT * T3_GLYPH_NUM)
	{
		t3_destroy_strarray(*lines_ptr);
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: %s: invalid file."
			T3_COLOR_RESET "\n", T3_GLYPH_FILE);
		exit(1);
	}
}

static size_t	count_dots(char **lines_of_glyph)
{
	size_t	row_index;
	size_t	n;

	row_index = 0;
	n = 0;
	while (row_index < T3_GLYPH_HEIGHT)
	{
		n += ft_cntchr(lines_of_glyph[row_index], '#');
		row_index += 1;
	}
	return (n);
}

// グリフの '#' の部分(dot)のみを点群データに変換する
static bool	vectorize_dots(t_glyph *glyph, char **lines_of_glyph)
{
	size_t		row_index;
	size_t		col_index;
	size_t		dots;

	glyph->n_points = count_dots(lines_of_glyph);
	glyph->points = malloc(sizeof(t_vector3d) * glyph->n_points);
	if (!glyph->points)
		return (false);
	row_index = 0;
	dots = 0;
	while (row_index < T3_GLYPH_HEIGHT)
	{
		col_index = 0;
		while (lines_of_glyph[row_index][col_index])
		{
			if (lines_of_glyph[row_index][col_index] == '#')
				memcpy(glyph->points[dots++], (t_vector3d){
					col_index * T3_GLYPH_SCALE,
					(T3_GLYPH_HEIGHT - row_index) * T3_GLYPH_SCALE,
					0}, sizeof(t_vector3d));
			col_index += 1;
		}
		row_index += 1;
	}
	return (true);
}

// 各グリフから点群データを生成する
bool	vertorize_glyphs(char **lines, t_glyph *glyphs)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < T3_GLYPH_NUM)
	{
		if (!vectorize_dots(&glyphs[i], &lines[i * T3_GLYPH_HEIGHT]))
		{
			j = 0;
			while (j < i)
			{
				free(glyphs[j].points);
				j += 1;
			}
			return (false);
		}
		i += 1;
	}
	return (true);
}

// グリフファイルからグリフを読み取り、t_glyph配列に格納する。
// 正常に完了したらtrueを返す。
bool	t3_read_glyphs(t_glyph *glyphs)
{
	char	**lines;
	bool	successfully_vectorized;

	lines = t3_read_all_lines(T3_GLYPH_FILE);
	if (!lines)
		return (false);
	validate_shape_of_glyph_file(&lines);
	successfully_vectorized = vertorize_glyphs(lines, glyphs);
	t3_destroy_strarray(lines);
	return (successfully_vectorized);
}
