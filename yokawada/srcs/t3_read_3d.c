/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_read_3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 22:47:34 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 11:06:57 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// doubleをビット操作するための共用体
typedef union u_ull_double
{
	unsigned long long	ull;
	double				dbl;
}	t_ull_double;

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
// 与えられたdoubleが有限かどうかを判定する
bool	rd_is_finite(const double val)
{
	t_ull_double		u;
	unsigned long long	fullbit;

	fullbit = (1ull << 11) - 1ull;
	u.dbl = val;
	return (((u.ull >> 52) & fullbit) != fullbit);
}

// nanを返す
double	rd_nan(void)
{
	t_ull_double	u;

	u.ull = (1ull << 63) - (1ull << 52) + 1;
	return (u.dbl);
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

// カンマ区切りの文字列をベクトル(double,double,double)に変換する。失敗したらfalseを返す。
bool	t3_vectorize(const char *str, t_vector3d vector)
{
	char			**splitted;

	splitted = ft_rawsplit(str, ',');
	if (!splitted)
		return (false);
	if (t3_count_lines(splitted) == 3)
	{
		vector[0] = strtod(splitted[0], NULL);
		vector[1] = strtod(splitted[1], NULL);
		vector[2] = strtod(splitted[2], NULL);
		if (rd_is_finite(vector[0]) && rd_is_finite(vector[1])
			&& rd_is_finite(vector[2]))
		{
			rd_free_strarray(&splitted);
			return (true);
		}
	}
	rd_free_strarray(&splitted);
	return (false);
}

// file_path の中身を1つの文字列として読み取る
t_vector3d	*t3_read_from_file(const char *file_path)
{
	char		*content;
	char		**lines;
	t_vector3d	*points;
	size_t		n;
	size_t		i;
	
	content = rd_read_file_content(file_path);
	if (!content)
	{
		return (NULL);
	}
	lines = ft_split(content, '\n');
	free(content);
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
	points[n][0] = rd_nan();
	i = 0;
	double sx = 0;
	double sy = 0;
	double sz = 0;
	while (i < n)
	{
		if (!t3_vectorize(lines[i], points[i]))
		{
			// しっぱい
			return (NULL);
		}
		sx += points[i][0];
		sy += points[i][1];
		sz += points[i][2];
		i += 1;
	}
	// printf("centroid: (%f, %f, %f)\n", sx / n, sy / n, sz / n);
	return (points);
}
