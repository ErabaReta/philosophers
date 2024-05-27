/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:24:57 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/21 19:37:38 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	digits_counter(long n)
{
	int	count;

	if (n < 0)
		n *= -1;
	if (n == 0)
		return (1);
	count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	array_filler(char *str, int is_negative, long nbr, int len)
{
	while (nbr)
	{
	str[--len + is_negative] = (nbr % 10) + 48;
	nbr /= 10;
	}
	if (is_negative == 1)
		str[0] = '-';
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	int		is_negative;
	long	nbr;

	nbr = n;
	is_negative = 0;
	len = digits_counter(nbr);
	if (nbr < 0)
	{
		nbr *= -1;
		is_negative = 1;
	}
	str = (char *)malloc((len + is_negative + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[len + is_negative] = '\0';
	if (nbr == 0)
		str[0] = '0';
	array_filler(str, is_negative, nbr, len);
	return (str);
}