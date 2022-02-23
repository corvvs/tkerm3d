/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_timing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 22:51:36 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 11:35:40 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// returns current time in micro seconds.
t_ut	t3_get_ut(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		return (0);
	}
	return ((tv.tv_sec) * 1e6 + tv.tv_usec);
}

// sleep until `this_time` minutely.
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
