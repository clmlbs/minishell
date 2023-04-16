/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:04:55 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 16:26:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_count
{
	unsigned int	count;
	unsigned int	j;
	unsigned int	start;
	size_t			len;
}	t_count;

char		*ft_itoa(int n);
void 		perror_void(char *str);
void		free_tab_strs(char **str);
void		ft_putchar_fd(char c, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s1);
char		*ms_strjoin(char const *s1, char const *s2);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ms_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_tolower(int c);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_strzero(char *s, size_t n);
char		*ft_substr(char const *s, unsigned int start, size_t len);
long long	ms_atoi(const char *str);
char		**ms_split(char *s, char c);

#endif