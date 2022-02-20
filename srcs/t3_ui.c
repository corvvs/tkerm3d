/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_ui.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 10:37:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/19 11:10:31 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

void	t3_repoint(t_system *system)
{
	size_t	size;

	if (system->points_animated)
	{
		free(system->points_animated);
	}
	size = sizeof(t_vector3d) * (system->n_points + 1);
	system->points_animated = malloc(size);
}

// n個の位置ベクトルを、重心が(0, 0, 0)になるように平行移動する。
void	t3_centralize_points(size_t n, t_vector3d *points)
{
	size_t		i;
	t_vector3d	c;

	bzero(&c, sizeof(double) * 3);
	i = 0;
	while (i < n)
	{
		c[0] += points[i][0];
		c[1] += points[i][1];
		c[2] += points[i][2];
		i += 1;
	}
	c[0] /= n;
	c[1] /= n;
	c[2] /= n;
	i = 0;
	while (i < n)
	{
		points[i][0] -= c[0];
		points[i][1] -= c[1];
		points[i][2] -= c[2];
		i += 1;
	}
}

// キーが押されていたら押されたキーを返す
// そうでなければ0を返す
int	t3_get_key(void)
{
	struct termios	oldt;
	struct termios	newt;
	int				ch;
	int				oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO | ECHOE);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF)
		return (ch);
	return (0);
}

void	t3_update_by_key(t_system *system)
{
	const int	key = t3_get_key();

	if (!key)
		return ;
	if (strchr("eq", key))
		t3_update_omega(system, key);
	else if (strchr("wasd", key))
		t3_update_offset(system, key);
	else if (strchr("xz", key))
		t3_update_flame(system, key);
	else if (strchr("12", key))
		t3_update_scale(system, key);
	else if (strchr("345", key))
		t3_udpate_axis(system, key);
	else if (key == ' ')
		t3_update_animate(system);
	else if (key == 'r')
		t3_reset_params(system);
	else if (key == T3_CHAR_ESC && system->src_mode == T3_SRC_TEXT)
		t3_scan_message(system);
	else
		dprintf(STDERR_FILENO, "key is %d('%c')\n", key, key);
}
