#include "philo.h"

long	ft_atoi(const char *str)
{
	size_t			i;
	size_t			nbr;
	int				sign;

	i = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	nbr = 0;
	while (str[i] != '\0' && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr *= 10;
		nbr += str[i++] - 48;
	}
	return (nbr * sign);
}