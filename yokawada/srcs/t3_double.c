/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_double.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 00:46:28 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 00:47:55 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// doubleをビット操作するための共用体
typedef union u_ull_double
{
	unsigned long long	ull;
	double				dbl;
}	t_ull_double;

// 与えられたdoubleが有限かどうかを判定する
bool	t3_is_fintie(const double val)
{
	t_ull_double		u;
	unsigned long long	fullbit;

	fullbit = (1ull << 11) - 1ull;
	u.dbl = val;
	return (((u.ull >> 52) & fullbit) != fullbit);
}

// nanを返す
double	t3_nan(void)
{
	t_ull_double	u;

	u.ull = (1ull << 63) - (1ull << 52) + 1;
	return (u.dbl);
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
		if (t3_is_fintie(vector[0]) && t3_is_fintie(vector[1])
			&& t3_is_fintie(vector[2]))
		{
			rd_free_strarray(&splitted);
			return (true);
		}
	}
	rd_free_strarray(&splitted);
	return (false);
}
