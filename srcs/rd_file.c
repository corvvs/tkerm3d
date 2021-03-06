/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:36:10 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/22 14:22:44 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rd_file.h"

static void	*exit_with_error(const char *error)
{
	dprintf(STDERR_FILENO, T3_COLOR_YELLOW
		"Error: %s"
		T3_COLOR_RESET "\n", error);
	exit(EXIT_FAILURE);
}

static void	deploy_buffer(t_plastic_buffer *buffer)
{
	char	*new_body;

	if (buffer->cap > 0)
		buffer->cap = buffer->cap * 2;
	else
		buffer->cap = FT_BUFSIZ;
	new_body = (char *)malloc((buffer->cap + 1) * sizeof(char));
	if (!new_body)
	{
		free(buffer->body);
		return ;
	}
	if (new_body)
	{
		memcpy(new_body, buffer->body, buffer->used);
		free(buffer->body);
	}
	buffer->body = new_body;
}

char	*t3_read_from_fd(const int fd)
{
	char				read_buf[FT_BUFSIZ];
	ssize_t				read_size;
	t_plastic_buffer	data_buf;

	bzero(&data_buf, sizeof(t_plastic_buffer));
	while (1)
	{
		read_size = read(fd, read_buf, FT_BUFSIZ);
		if (read_size <= 0)
			break ;
		if (data_buf.used + read_size > data_buf.cap)
			deploy_buffer(&data_buf);
		if (!data_buf.body)
			return (NULL);
		memcpy(data_buf.body + data_buf.used, read_buf, read_size);
		data_buf.used += read_size;
	}
	if (read_size < 0 || !data_buf.body)
	{
		free(data_buf.body);
		return (NULL);
	}
	data_buf.body[data_buf.used] = '\0';
	return (data_buf.body);
}

char	*rd_read_file_content(const char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		exit_with_error("failed to open file");
	content = t3_read_from_fd(fd);
	if (!content)
		exit_with_error("failed to read file content");
	if (close(fd) == -1)
	{
		free(content);
		exit_with_error("failed to close file");
	}
	return (content);
}
