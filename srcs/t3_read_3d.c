/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_read_3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 22:47:34 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 11:06:13 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

char	**t3_read_all_lines(const char *file_path)
{
	char		*content;
	char		**lines;

	content = rd_read_file_content(file_path);
	if (!content)
	{
		return (NULL);
	}
	lines = ft_split(content, '\n');
	free(content);
	if (!lines)
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: %s: failed to split."
			T3_COLOR_RESET "\n", file_path);
		return (NULL);
	}
	return (lines);
}

// 文字列配列を破壊する
void	t3_destroy_strarray(char **strs)
{
	size_t	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

// 文字列配列の長さを求める
size_t	t3_count_lines(char **lines)
{
	size_t	n;

	n = 0;
	while (lines[n])
	{
		n += 1;
	}
	return (n);
}

// linesの中身をベクトルに変換してpointsに詰める。最大n個。変換できた個数を返す。
size_t	vectorize_lines_into_points(size_t n, char **lines, t_vector3d *points)
{
	size_t		i;

	i = 0;
	points[n][i] = NAN;
	while (i < n)
	{
		if (!t3_vectorize(lines[i], points[i]))
			break ;
		i += 1;
	}
	return (i);
}

t_vector3d	*t3_read_vectors_from_file(const char *file_path)
{
	char		**lines;
	t_vector3d	*points;
	size_t		n;
	size_t		i;

	lines = t3_read_all_lines(file_path);
	if (!lines)
		return (NULL);
	n = t3_count_lines(lines);
	i = 0;
	points = malloc(sizeof(t_vector3d) * (n + 1));
	if (points)
		i = vectorize_lines_into_points(n, lines, points);
	t3_destroy_strarray(lines);
	if (i < n)
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: %s: %zu: invalid format."
			T3_COLOR_RESET "\n", file_path, i + 1);
		free(points);
		return (NULL);
	}
	return (points);
}
