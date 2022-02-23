/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t3_update_bezier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:15:17 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/22 19:47:55 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

static char	*getline_for_bezier(void)
{
	char	*input_str;
	size_t	n;
	size_t	rs;

	dprintf(STDERR_FILENO,
		"input cubic-bezier parameters in format: "
		"\"%%f %%f %%f %%f\"\n");
	n = 0;
	input_str = NULL;
	rs = getline(&input_str, &n, stdin);
	if (!input_str || rs < 1)
	{
		free(input_str);
		return (NULL);
	}
	input_str[rs - 1] = '\0';
	return (input_str);
}

static char	**input_for_bezier(void)
{
	char	*input_str;
	char	**splitted;

	input_str = getline_for_bezier();
	if (!input_str)
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: failed to read from stdin." T3_COLOR_RESET "\n");
		return (NULL);
	}
	splitted = ft_split(input_str, ' ');
	free(input_str);
	if (!splitted)
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: failed to read from stdin." T3_COLOR_RESET "\n");
	}
	return (splitted);
}

void	t3_update_bezier(t_system *system)
{
	char	**splitted;

	splitted = input_for_bezier();
	if (!splitted)
		return ;
	if (t3_strs_to_cubic_bezier(splitted, &system->optics.bezier))
		t3_setup_cubic_bezier(&system->optics.bezier);
	else
	{
		dprintf(STDERR_FILENO, T3_COLOR_YELLOW
			"Error: failed to parse as doubles" T3_COLOR_RESET "\n");
	}
	t3_destroy_strarray(splitted);
}
