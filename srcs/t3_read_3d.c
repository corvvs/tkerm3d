/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_read_3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 22:47:34 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 00:47:55 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 文字列配列を破壊する
void	rd_free_strarray(char ***pstrs)
{
	size_t	i;
	char	**strs;

	if (pstrs)
	{
		strs = *pstrs;
		if (strs)
		{
			i = 0;
			while (strs[i])
				free(strs[i++]);
			free(strs);
		}
		*pstrs = NULL;
	}
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

char	**t3_read_lines(const char *file_path)
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
	return (lines);
}

// file_path の中身を1つの文字列として読み取る
t_vector3d	*t3_read_vectors_from_file(const char *file_path)
{
	char		**lines;
	t_vector3d	*points;
	size_t		n;
	size_t		i;

	lines = t3_read_lines(file_path);
	if (!lines)
	{
		return (NULL);
	}
	n = t3_count_lines(lines);
	points = malloc(sizeof(t_vector3d) * (n + 1));
	if (!points)
	{
		return (NULL);
	}
	points[n][0] = t3_nan();
	i = 0;
	while (i < n)
	{
		if (!t3_vectorize(lines[i], points[i]))
		{
			return (NULL);
		}
		i += 1;
	}
	return (points);
}
