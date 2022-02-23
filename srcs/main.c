/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/23 18:04:56 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

int	main(int argc, char **argv)
{
	t_system	system;

	t3_check_tty_out();
	bzero(&system, sizeof(t_system));
	t3_read_source(&system, argc, argv);
	t3_setup_system(&system);
	t3_render_loop(&system);
}
