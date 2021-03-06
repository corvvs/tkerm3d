/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_file.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:37:12 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 21:52:10 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RD_FILE_H
# define RD_FILE_H

# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# define FT_BUFSIZ 4096
# include "t3_color.h"

typedef struct s_plastic_buffer
{
	char	*body;
	size_t	cap;
	size_t	used;
}	t_plastic_buffer;

#endif
