/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_offset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:25 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:01:27 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_offset(t_system *system, int key)
{
	if (key == 'd' || key == 'a')
	{
		system->optics.offset_x += ((key == 'd') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.x -> %f\n", system->optics.offset_x);
	}
	else if (key == 's' || key == 'w')
	{
		system->optics.offset_y += ((key == 's') * 2 - 1) * 0.1;
		dprintf(STDERR_FILENO, "offset.y -> %f\n", system->optics.offset_x);
	}
}
