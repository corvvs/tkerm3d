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

char **t3_glyph_array(const char *filename)
{
	char	*content;
	char	**lines;

	content = rd_read_file_content(filename);
	if (!content)
		return (NULL);
	lines = ft_split(content, '\n');
	free(content);
	return lines;
}

size_t t3_count_glyph_dots(char **lines, size_t start, size_t end)
{
	size_t row_index;
	size_t col_index;
	size_t dots;

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
	return dots;
}

// グリフファイルからグリフを読み取り、t_glyph配列に格納する。
// 読み取れたグリフの数を返す(最大でT3_GLYPH_NUM)。
size_t	t3_read_glyph(t_glyph *glyphs)
{
	char	**lines;
	size_t	k;
	size_t	j;
	size_t	dots;
	size_t	g;   // グリフのインデックス
	size_t	i;   // linesのインデックス
	size_t	i0;  // linesにおけるグリフのインデックス

	lines = t3_glyph_array("./printables.txt");
	if (!lines)
		return (0);
	g = 0;
	i = 0;
	i0 = i;
	while (lines[i])
	{
		// 次のグリフの開始インデックスまでスキップする
		i += 1;
		if (i % T3_GLYPH_HEIGHT != 0)
			continue ;

		// glyphsに点群を設定する
		glyphs[g].n_points = t3_count_glyph_dots(lines, i0, i);
		glyphs[g].points = malloc(sizeof(t_vector3d) * glyphs[g].n_points);
		j = i0;
		dots = 0;
		while (j < i)
		{
			k = 0;
			while (lines[j][k])
			{
				if (lines[j][k] == '#')
				{
					glyphs[g].points[dots][0] = k * T3_GLYPH_SCALE;
					glyphs[g].points[dots][1] = (i - j) * T3_GLYPH_SCALE;
					glyphs[g].points[dots][2] = 0;
					dots += 1;
				}
				k += 1;
			}
			j += 1;
		}
		i0 = i;
		g += 1;

		// 全てのグリフの読み取りが完了したら終了
		if (g == T3_GLYPH_NUM)
			break ;
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

void	t3_str_to_points(t_system *system, const char *str)
{
	size_t	i;
	size_t	l;
	size_t	n;
	size_t	j;
	size_t	n_points;

	if (system->points_original)
	{
		free(system->points_original);
		system->points_original = NULL;
	}
	n_points = 0;
	i = 0;
	n = 0;
	l = 0;
	while (str[i])
	{
		if (' ' <= str[i] && (size_t)str[i] < ' ' + system->n_glyphs)
			n_points += system->glyphs[str[i] - ' '].n_points;
		i += 1;
	}
	system->n_points = n_points;
	system->points_original = malloc(sizeof(t_vector3d) * (n_points + 1));
	n_points = 0;
	i = 0;
	n = 0;
	while (str[i])
	{
		if (isprint(str[i]))
		{
			memcpy(system->points_original + n_points,
				system->glyphs[str[i] - ' '].points,
				sizeof(t_vector3d) * system->glyphs[str[i] - ' '].n_points
				);
			j = 0;
			while (j < system->glyphs[str[i] - ' '].n_points)
			{
				system->points_original[n_points + j][0]
					+= T3_GLYPH_SCALE * T3_GLYPH_WIDTH * n;
				system->points_original[n_points + j][1]
					-= T3_GLYPH_SCALE * T3_GLYPH_HEIGHT * l;
				j += 1;
			}
			n_points += system->glyphs[str[i] - ' '].n_points;
			n += 1;
		}
		else if (str[i] == '\n')
		{
			n = 0;
			l += 1;
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
