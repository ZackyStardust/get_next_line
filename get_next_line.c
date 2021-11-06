/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpereira <fpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:37:26 by fpereira          #+#    #+#             */
/*   Updated: 2021/11/06 19:20:48 by fpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *feed_storage(char **storage, char **buf, size_t i);
static char *valid_buffer(char **storage, char **buf, size_t i);
static char	*valid_storage(char **storage, size_t size);

char *get_next_line(int fd)
{
	static char	*storage;
	char		*buf;
	char		*ret;
	size_t		i;

/*		This read is necessary 		|												*/
/*		to check if the file is 	|												*/
/*		exists or is valid. The		|												*/
/*		NULL (0) in the second		|												*/
/*		slot means that everything	|												*/
/*		that was read will not be	|												*/
/*		copied to anything. The 0	|												*/
/*		at the third slot means		|	The minimum value	|						*/
/*		nothing should be read.		|	for a file descrip-	|						*/
/*		A normal return should		|	tor is 0. -1 means	|						*/
/*		be 0 (nothing was read).	|	an error. The max-	|						*/
/*		A return of -1 means that	|	imum value of a		|	A buffer of 0		*/
/*		the function was not able	|	file descriptor is	|	or lower means		*/
/*		to read anything at all 	|	1024. Anything 		|	nothing will be		*/
/*		even if it wanted to.		|	greater is invalid.	|	read each time.		*/

	if ((read(fd, NULL, 0) == -1) || (fd < 0 || fd > 1024) || BUFFER_SIZE <= 0)
		return ERROR;
	i = 1;
	ret = NULL;	//	NULL because it must be initiated as something or the flags will scream.
	buf = ft_strchr(storage, '\n');	//	Now we check if there is anything inside the storage and if there is a linebreak.
	if (!buf)	//	If the storage was empty or if there was no linebreak we must feed the buffer to find the next newline.
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));	//	Allocate the memory necessary for the buffer: BUFFER_SIZE characters + 1 free space for a null at the end.
		if (!buf)
			return ERROR;
/*																			*/
/*	From here onwards, things start to get tricky, but bear with me.		*/
/*	1) Because of the feed_storage function, the return will only cease		*/
/*	to be NULL if the buffer finds a newline.								*/
/*	2) The i being greater than zero means that there it has actually read	*/
/*	_i_ bytes of data. Therefore, there are still things to be read inside	*/
/*	the file. If the i is zero, the feed_storage function will inject		*/
/*	everything that remained inside the ret pointer. The while loop will	*/
/*	break and ret will be returned.											*/
/*																			*/
		while (ret == NULL && i > 0)
		{
			i = read(fd, buf, BUFFER_SIZE);	//	We read BUFFER_SIZE amount of characters and store everything in the buf pointer.
			ret = feed_storage(&storage, &buf, i);	//	Now we will feed the storage with everything the buffer found and maybe feed the return with the storage and/or the buffer.
		}
		return (ret);
	}
//	ret = valid_storage(&storage, (buf - storage) + 1);
	return (valid_storage(&storage, (buf - storage) + 1));
}

static char *feed_storage(char **storage, char **buf, size_t i)
{
	char *tmp;
	char *ret;
	if (i <= 0)
	{
		if (i == 0)
		{
			ret = *storage;
			free(*storage);
		}
		return(ret);
	}
	(*buf)[i] = '\0';
	*tmp = ft_strchr(*buf, '\n');
	if (tmp)
	{
		ret = valid_buffer(storage, buf, (tmp - *buf) + 1);
	}
	else
	{
		tmp = ft_strjoin(*storage, *buf);
		if (*storage)
			free(*storage);
		*storage = tmp;
	}
	return (ret);
}

static char *valid_buffer(char **storage, char **buf, size_t buflen) // _buflen_ is the total size of valid _buf_ bytes necessary for a valid return (a full line) after the concatenation of _storage_ + _buf_.
{
	char *ret;
	char *tmp;
	size_t i;

	i = ft_strlen(*storage);
	ret = malloc(sizeof(char) * (buflen + 1));
	if (!ret)
		return ERROR;
	ft_memcpy(ret, *storage, i);
	ft_memcpy(ret + i, *buf, buflen);
	tmp = ft_strdup((*buf) + buflen);
	free(*storage);
	*storage = tmp;
	return (ret);
}

static char	*valid_storage(char **storage, size_t size)
{
	char *tmp;
	char *ret;
	size_t i;

	if (size < 0)
		return ERROR;
	tmp = malloc(sizeof(char) * (size + 1));
	if (!tmp)
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
