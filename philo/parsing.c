/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 17:03:30 by eouhrich          #+#    #+#             */
/*   Updated: 2024/07/27 19:09:51 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parsing(t_vars *vars, char **av, int ac)
{
	if (ac != 5 && ac != 6)
		return (-1);
	vars->number_of_philosophers = ft_philo_atoi(av[1]);
	if (vars->number_of_philosophers == 0)
		return (-1);
	vars->time_to_die = ft_philo_atoi(av[2]);
	vars->time_to_eat = ft_philo_atoi(av[3]);
	if (vars->time_to_eat == 0)
		vars->time_to_eat = 1;
	vars->time_to_sleep = ft_philo_atoi(av[4]);
	if (ac == 6)
		vars->times_must_eat = ft_philo_atoi(av[5]);
	else
		vars->times_must_eat = -1;
	if (vars->number_of_philosophers == 2147483648
		|| vars->time_to_die == 2147483648 || vars->time_to_eat == 2147483648
		|| vars->time_to_sleep == 2147483648
		|| vars->times_must_eat == 2147483648)
	{
		printf("Error: bad argument\n");
		return (-1);
	}
	if (vars->times_must_eat == 0)
		return (-1);
	return (0);
}
