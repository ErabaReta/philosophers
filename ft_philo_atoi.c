/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_atoi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:38 by eouhrich          #+#    #+#             */
/*   Updated: 2024/06/10 22:24:45 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	sign_giver(char *str, size_t *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign *= -1;
		*i += 1;
	}
	return (sign);
}

long	ft_philo_atoi(const char *str)
{
	size_t			i;
	long long			nbr;
	int				sign;
	int				counter;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign = sign_giver((char *)str, &i);
	if (str[i] == '+' || str[i] == '-')
		return (2147483648);
	nbr = 0;
	counter = 0;
	while (str[i] != '\0' && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = nbr * 10 + str[i++] - 48;
		if ((nbr >= 2147483648 && sign == 1)
			|| (nbr >= 2147483649 && sign == -1))
			return (2147483648);
		counter++;
	}
	if ((str[i] != '\0') || (counter < 1) || (nbr * sign < 0))
		return (2147483648);
	return (nbr * sign);
}
