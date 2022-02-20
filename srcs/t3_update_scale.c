/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_scale.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:34 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:01:36 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_scale(t_system *system, int key)
{
	if (key == '1')
	{
		system->optics.scale_factor /= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
	else if (key == '2')
	{
		system->optics.scale_factor *= 1.05;
		dprintf(STDERR_FILENO, "scale -> %f\n", system->optics.scale_factor);
	}
}
