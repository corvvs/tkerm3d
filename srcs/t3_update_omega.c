/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_omega.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:31 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/23 11:44:26 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_omega(t_system *system, int key)
{
	if (key == 'e' || key == 'q')
	{
		system->optics.omega += ((key == 'e') * 2 - 1) * 0.1;
		dprintf(
			STDERR_FILENO,
			"omega -> %1.2f[rot/s]\n",
			system->optics.omega
			);
	}
}
