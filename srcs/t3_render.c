/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:43 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:02:45 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

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

char	t3_char_for_pixel(size_t count)
{
	if (count > 3)
		return ('#');
	if (count > 1)
		return ('*');
	if (count > 0)
		return ('.');
	return (' ');
}

// ピクセルバッファを表示
void	t3_render(t_system *system)
{
	char	*buffer;
	size_t	i;
	size_t	j;

	buffer = system->pixelbuffer;
	t3_clear_pixelbuffer(system);
	t3_fill_pixelbuffer(system);
	write(STDOUT_FILENO, T3_ES_CLEAR, 4);
	i = 0;
	while (i < system->optics.height)
	{
		j = 0;
		while (j < system->optics.width)
		{
			buffer[i * (system->optics.width + 1) + j]
				= t3_char_for_pixel(system->optics.pixels[i][j]);
			j += 1;
		}
		buffer[i * (system->optics.width + 1) + j] = '\n';
		i += 1;
	}
	write(STDOUT_FILENO, buffer,
		system->optics.height * (system->optics.width + 1));
}
