/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_flame.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:22 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:01:23 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_flame(t_system *system, int key)
{
	if (key == 'x' || key == 'z')
	{
		if ((key == 'x' && system->optics.fps < 240)
			|| (key == 'z' && 2 < system->optics.fps))
			system->optics.fps += ((key == 'x') * 2 - 1);
		system->optics.uspf = 1000000 / system->optics.fps;
		dprintf(STDERR_FILENO, "fps -> %f\n", system->optics.fps);
	}
}
