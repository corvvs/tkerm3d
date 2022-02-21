/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_double.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 00:46:28 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 11:58:53 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// doubleをビット操作するための共用体
typedef union u_ull_double
{
	unsigned long long	ull;
	double				dbl;
}	t_ull_double;

// str を strtod でdoubleに変換する。
// 失敗した場合はnanを返す。
double	strtod_wrapped(const char *str)
{
	char	*check_ptr;
	double	d;

	if (!str || !str[0])
		return (NAN);
	check_ptr = NULL;
	d = strtod(str, &check_ptr);
	if (check_ptr && check_ptr[0])
		return (NAN);
	return (d);
}

// カンマ区切りの文字列をベクトル(double,double,double)に変換する。失敗したらfalseを返す。
bool	t3_vectorize(const char *str, t_vector3d vector)
{
	char			**splitted;

	splitted = ft_rawsplit(str, ',');
	if (!splitted)
		return (false);
	if (t3_count_lines(splitted) != 3)
	{
		t3_destroy_strarray(splitted);
		return (false);
	}
	vector[0] = strtod_wrapped(splitted[0]);
	vector[1] = strtod_wrapped(splitted[1]);
	vector[2] = strtod_wrapped(splitted[2]);
	t3_destroy_strarray(splitted);
	return (isfinite(vector[0]) && isfinite(vector[1])
		&& isfinite(vector[2]));
}
