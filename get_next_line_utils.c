/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpereira <fpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 13:37:25 by fpereira          #+#    #+#             */
/*   Updated: 2021/11/02 13:44:41 by fpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(const char *src)
{
	char	*str;
	int		len;
	int		index;

	index = 0;
	len = ft_strlen(src);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (0);
	while (src[index] != '\0')
	{
		str[index] = src[index];
		index++;
	}
	str[index] = '\0';
	return (str);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!src && !dst)
		return (0);
	i = 0;
	while (i < n)
	{
		((char *)dst)[i] = ((const char *)src)[i];
		i++;
	}
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	ii;
	char	*s3;

	if (!s1 || !s2)
		return (0);
	s3 = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!s3)
		return (0);
	i = 0;
	ii = 0;
	while (s1[ii])
		s3[i++] = s1[ii++];
	ii = 0;
	while (s2[ii])
		s3[i++] = s2[ii++];
	s3[i] = '\0';
	return (s3);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != (unsigned char)c && *s)
		s++;
	if (*s == (unsigned char)c)
		return ((char *)s);
	else
		return (0);
}
