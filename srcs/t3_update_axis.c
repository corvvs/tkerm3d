/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_axis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:18 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/22 14:14:57 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_axis(t_system *system, int key)
{
	t_vector3d	*axis;

	axis = &system->optics.rot_axis;
	if (key == '3')
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){1, 0, 0}, M_PI / 256);
	else if (key == '4')
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 1, 0}, M_PI / 256);
	else if (key == '5')
		t3_rot_vector_around_axis(system->optics.rot_axis,
			(t_vector3d){0, 0, 1}, M_PI / 256);
	else
		return ;
	dprintf(STDERR_FILENO, "rotation axis -> (%f, %f, %f)\n",
		system->optics.rot_axis[0],
		system->optics.rot_axis[1],
		system->optics.rot_axis[2]);
}
