/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_timing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 22:51:36 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/18 22:52:54 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 現在時刻をマイクロ秒単位で返す
t_ut	t3_get_ut(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		return (0);
	}
	return ((tv.tv_sec) * 1e6 + tv.tv_usec);
}

// this_time を超えるまで1000マイクロ秒ずつusleepする。
// その後、現在時刻を返す。
t_ut	t3_wait_until(t_ut this_time)
{
	t_ut	t1;

	while (true)
	{
		t1 = t3_get_ut();
		if (this_time <= t1)
		{
			break ;
		}
		usleep(1000);
	}
	return (t1);
}
