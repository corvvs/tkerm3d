/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_omega.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:31 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:01:36 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_omega(t_system *system, int key)
{
	if (key == 'e' || key == 'q')
	{
		system->optics.omega += ((key == 'e') * 2 - 1) * 0.000001;
		dprintf(
			STDERR_FILENO,
			"omega -> %f[rot/s]\n",
			system->optics.omega * 1000000 / M_PI / 2
			);
	}
}
