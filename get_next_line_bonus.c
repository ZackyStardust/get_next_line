/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpereira <fpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:37:26 by fpereira          #+#    #+#             */
/*   Updated: 2021/11/07 12:09:48 by fpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

// Our .h is the same. Our _utils.c is also the same. Don't worry about it.

static char *feed_storage(char **storage, char **buf, size_t i);
static char *valid_buffer(char **storage, char **buf, size_t i);
static char	*valid_storage(char **storage, size_t size);

char *get_next_line(int fd)
{
	static char	*storage[1024];	//	We now prepare the storage as an array with the maximum size of fd possible.
	char		*buf;
	char		*ret;
	size_t		i;
	if ((read(fd, NULL, 0) == -1) || (fd < 0 || fd > 1024) || BUFFER_SIZE <= 0)
		return ERROR;
	i = 1;
	ret = NULL;
	buf = ft_strchr(storage[fd], '\n');	//	And whenever we need to use the storage array, we use the fd as its index.
	if (!buf)
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return ERROR;
		while (ret == NULL && i > 0)
		{
			i = read(fd, buf, BUFFER_SIZE);
			ret = feed_storage(&storage[fd], &buf, i);	//	See? Again.
		}
		free (buf);
		return (ret);
	}
	ret = valid_storage(&storage[fd], (buf - storage[fd]) + 1);	//	And once more. After here, we don't need to worry any more. Seriously, it is DONE.
	return (ret);
}

static char *feed_storage(char **storage, char **buf, size_t i)
{
	char *tmp;
	char *ret;

	ret = NULL;
	if (i <= 0)
	{
		if (i == 0 && *storage)
		{
			ret = (*storage);
			*storage = NULL;
		}
		return (ret);
	}
	(*buf)[i] = '\0';
	tmp = ft_strchr(*buf, '\n');
	if (tmp)
		ret = valid_buffer(storage, buf, (tmp - *buf) + 1);
	else
	{
		tmp = ft_strjoin(*storage, *buf);
		if (*storage)
			free(*storage);
		*storage = tmp;
	}
	return (ret);
}

static char *valid_buffer(char **storage, char **buf, size_t buflen)
{
	char *ret;
	char *tmp;
	size_t i;

	i = ft_strlen(*storage);
	ret = (char *)malloc(sizeof(char) * (buflen + i + 1));
	if (!ret)
		return ERROR;
	ft_memcpy(ret, *storage, i);
	ft_memcpy(ret + i, *buf, buflen);
	ret[i + buflen] = '\0';
	tmp = ft_strdup((*buf) + buflen);
	if (*storage)
		free(*storage);
	(*storage) = tmp;
	return (ret);
}

static char	*valid_storage(char **storage, size_t size)
{
	char *tmp;
	char *ret;
	size_t i;

	if (size < 0)
		return ERROR;
	ret = malloc(sizeof(char) * (size + 1));
	if (!ret)
		return ERROR;
	i = -1;
	while (++i != size)
		ret[i] = (*storage)[i];
	ret[i] = '\0';
	tmp = ft_strdup(*storage + i);
	free (*storage);
	(*storage) = tmp;
	return (ret);
}
