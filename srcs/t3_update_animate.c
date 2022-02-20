/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_animate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomatsu <tkomatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:01:10 by tkomatsu          #+#    #+#             */
/*   Updated: 2022/02/20 16:01:11 by tkomatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_update_animate(t_system *system)
{
	system->optics.animate = !system->optics.animate;
	dprintf(STDERR_FILENO, "animate -> %s\n",
		(char *[2]){"NO", "YES"}[!!system->optics.animate]);
}
