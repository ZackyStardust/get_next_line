/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpereira <fpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:37:26 by fpereira          #+#    #+#             */
/*   Updated: 2021/11/07 13:54:10 by fpereira         ###   ########.fr       */
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

//	      ** THE READ **
//	   This read is necessary      |
//	   to check if the file is     |
//	   exists or is valid. The     |
//	   NULL (0) in the second      |
//	   slot means that everything  |
//	   that was read will not be   |
//	   copied to anything. The 0   |   ** FD CHECK **
//	   at the third slot means     |   The minimum value    |
//	   nothing should be read.     |   for a file descrip-  |
//	   A normal return should      |   tor is 0. -1 means   |   ** BUFFER_SIZE **
//	   be 0 (nothing was read).    |   an error. The max-   |     ** CHECK **
//	   A return of -1 means that   |   imum value of a      |    A buffer of 0
//	   the function was not able   |   file descriptor is   |    or lower means
//	   to read anything at all     |   1024. Anything       |    nothing will be	
//	   even if it wanted to.       |   greater is invalid.  |    read each time.	
	if ((read(fd, NULL, 0) == -1) || (fd < 0 || fd > 1024) || BUFFER_SIZE <= 0)
		return ERROR; // My ERROR is defined as (0) in my .h. It is just my aesthetic preference.
	i = 1;
	ret = NULL;	// NULL because it must be initiated as something or the flags will scream.
	buf = ft_strchr(storage, '\n');	// Now we check if there is anything inside the storage and if there is a linebreak.
	if (!buf)	// If the storage was empty or if there was no linebreak we must feed the buffer to find the next newline.
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));	// Allocate the memory necessary for the buffer: BUFFER_SIZE characters + 1 free space for a null at the end.
		if (!buf)
			return ERROR;
//	From here onwards, things start to get tricky, but bear with me.

//	1) Because of the feed_storage function, the return will only cease
//	to be NULL if the buffer finds a newline.

//	2) The i being greater than zero means that it has actually read
//	_i_ bytes of data. Therefore, there are still things to be read inside
//	the file (or not). If the i is zero, it means that there is nothing
//	else to read. It also means that the buffer is empty and that everything
//	that remained to be returned is already stored inside the Storage.
//	Therefore, the return will be filled and the loop will break.
		while (ret == NULL && i > 0)
		{
			i = read(fd, buf, BUFFER_SIZE);	// We read BUFFER_SIZE amount of characters and store everything in the buf pointer.
			ret = feed_storage(&storage, &buf, i);	// Now we will feed the storage with everything the buffer found and maybe feed the return with the storage and/or the buffer.
		}
		free (buf);	// We don't need this guy anymore because it was used and already transfered to the storage. See ya next loop, buf.
		return (ret);
	}
	ret = valid_storage(&storage, (buf - storage) + 1);
	// If there was a linebreak inside the Storage, we just need to put
	// everything up to that linebreak to our ret and then clear the
	// Storage's used content, leaving only the stuff _after_ the linebreak.
	return (ret);
}

static char *feed_storage(char **storage, char **buf, size_t i)
{
	char *tmp;
	char *ret;

	ret = NULL;	// Because of the flags, this variable has a chance to be returned without being assigned a value. Therefore, we assign a NULL (0) to it.
	if (i <= 0)	// If the i <= zero, it means that we either reached the end of the file or the file is invalid somehow.
	{
		if (i == 0 && *storage)   // As stated before, if it is zero, we reached the end,
		{                         // so we simply redirect the ret pointer to point to storage,
			ret = (*storage);     // and the storage pointer to NULL. We also verify if there is
			*storage = NULL;      // anything in Storage, because it might be empty if the file
		}                         // is valid but empty.
		return (ret);
	}
	(*buf)[i] = '\0';	// We will conduct a reading of the buffer string in the next line, so we need to null-terminate it.
	tmp = ft_strchr(*buf, '\n');   // We will try to find a newline inside the buffer and make tmp point to it.
	if (tmp)                       // If there is a newline in the buffer we call
//                                 // the valid_buffer function, sending the storage,
//                                 // the buffer and the address of tmp - the initial address
//                                 // of buf (i.e. the length of the string inside the buffer).
//                                 // +1 for the null-termination.
		ret = valid_buffer(storage, buf, (tmp - *buf) + 1);
	else                    // If there is not a newline in the buffer we concatenate it
	{                       // with the storage and send it to the tmp pointer.
		tmp = ft_strjoin(*storage, *buf);  // The ret will be returned as NULL and the while in
		if (*storage)                      // get_next_line will trigger again.
			free(*storage);
		*storage = tmp;
	}
	return (ret);
// If there ever was a newline inside the buffer, a valid ret will be returned.
// If not, the storage will be fed with buffer content and the ret will be
// null, meaning the loop will trigger once more.
}

static char *valid_buffer(char **storage, char **buf, size_t buflen)
// _buflen_ is the total size of valid _buf_ bytes necessary for a
// valid return (a full line) after the concatenation of _storage_ + _buf_.
{
	char *ret;
	char *tmp;
	size_t i;

	i = ft_strlen(*storage); // We must know the total size of the storage in order to copy it perfectly.
	ret = (char *)malloc(sizeof(char) * (buflen + i + 1));
// As always, a malloc for the return, gathering all the size of the buffer
// up to the first occurence of a newline.
	if (!ret)
		return ERROR;
	ft_memcpy(ret, *storage, i); // Copy everything from the storage...
	ft_memcpy(ret + i, *buf, buflen); // And copy buflen characters from the buffer to the ret in a space after the total size of storage.
	ret[i + buflen] = '\0'; // The null-termination in the slot after storage size and after buflen, meaning after the whole new line.
	tmp = ft_strdup((*buf) + buflen); // The good 'ol storage feeding, now feeding it with the remaining content of buf.
	if (*storage)
		free(*storage);
	(*storage) = tmp;
	return (ret);
}

static char	*valid_storage(char **storage, size_t size)
// This is the simplest of the situations. If there is a newline           //
// inside the storage, it means we don't even need to feed the buf         //
// with content from the file. We just need to use it to mark the          //
// exact address of the newline inside the storage string.                 //
// Then, we simply pick everything from the storage up to the              //
// newline, feed it to the ret, clean the storage of it and send it back.  //
{
	char *tmp;
	char *ret;
	size_t i;

	if (size < 0)		// If the size (address of the newline - the initial address of storage)
		return ERROR;	// is less than zero, something went wrong. Abort, abort!
	ret = malloc(sizeof(char) * (size + 1));
	if (!ret)
		return ERROR;
	i = -1;                     // In order to not use too many lines, we may start the index with -1
	while (++i != size)         // so we can increase it first, then compare to the size.
//                              // And while this loop condition is valid, we feed the return
		ret[i] = (*storage)[i]; // with the content from the storage.
	ret[i] = '\0';
	tmp = ft_strdup(*storage + i);  // We feed the temporary pointer with a string	
	free (*storage);                // duplication using storage, but only its address
	(*storage) = tmp;               // _after_ the newline. Then we clean it and make
	return (ret);                   // the storage point to the tmp string.
}

/* ---------------------------------------------------- */
/*  Now for a great question: why in some functions     */
/*  the pointer parameters are ** and not simply *?     */
/*  Surely only one asterisk would be enough, right?    */
/*  And you are correct, my inquisitive friend.         */
/*  However, this way our file is ready for the bonus!  */
/*  Check out the _bonus files to see how it works.     */
/* ---------------------------------------------------- */