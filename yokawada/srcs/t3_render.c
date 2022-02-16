/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 21:44:13 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 11:06:19 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 現在時刻をマイクロ秒単位で返す
t_ut	t3_get_ut(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
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
			break;
		}
		usleep(1000);
	}
	return (t1);
}

// ピクセルバッファをクリア
void	t3_clear_pixelbuffer(t_system *system)
{
	size_t	i;

	i = 0;
	while (i < system->optics.height)
	{
		bzero(system->optics.pixels[i], sizeof(size_t) * system->optics.width);
		i += 1;
	}
}

// ピクセルバッファをセット
void	t3_fill_pixelbuffer(t_system *system)
{
	size_t	i;
	double	x;
	double	y;
	size_t	xi;
	size_t	yi;

	i = 0;
	while (i < system->n_points)
	{
		x = system->points_animated[i][0];
		y = system->points_animated[i][1];
		if (x >= 0 && y >= 0)
		{
			xi = (size_t)(x / system->optics.sq_size_x);
			yi = (size_t)(y / system->optics.sq_size_y);
			if (xi < system->optics.width && yi < system->optics.height)
			{
				system->optics.pixels[yi][xi] += 1;
			}
		}
		i += 1;
	}
}

// ピクセルバッファを表示
void	t3_render_pixel_buffer(t_system *system)
{
	char	real_buffer[4 + T3_HEIGHT * (T3_WIDTH + 1)];
	char	*buffer;
	size_t	i;
	size_t	j;

	memcpy(real_buffer, "\x1b[H", 4);
	buffer = real_buffer + 4;
	i = 0;
	while (i < system->optics.height)
	{
		j = 0;
		while (j < system->optics.width)
		{
			if (system->optics.pixels[i][j] > 3)
				buffer[i * (system->optics.width + 1) + j] = '#';
			else if (system->optics.pixels[i][j] > 1)
				buffer[i * (system->optics.width + 1) + j] = '*';
			else if (system->optics.pixels[i][j] > 0)
				buffer[i * (system->optics.width + 1) + j] = '.';
			else
				buffer[i * (system->optics.width + 1) + j] = ' ';
			j += 1;
		}
		buffer[i * (system->optics.width + 1) + j] = '\n';
		i += 1;
	}
	write(STDOUT_FILENO, real_buffer, 4 + T3_HEIGHT * (T3_WIDTH + 1));
}
