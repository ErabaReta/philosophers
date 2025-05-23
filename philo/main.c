/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:33 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:26:23 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_finished(t_philo *philo)
{
	pthread_mutex_lock(&(philo->state_lock));
	if (philo->simulation_finished == 1)
	{
		pthread_mutex_unlock(&(philo->state_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo->state_lock));
	return (0);
}

size_t	get_time_milliseconds(struct timeval tv)
{
	gettimeofday(&(tv), NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	create_threads(t_vars *vars, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < vars->number_of_philosophers)
	{
		if (pthread_create(&(vars->philos[i]), NULL, routine, philo[i]) != 0)
		{
			printf("failed to create philo thread\n");
			return (-1);
		}
		i++;
	}
	vars->initial_timeval = get_time_milliseconds((*philo)->tv);
	i = -1;
	while (++i < vars->number_of_philosophers)
		philo[i]->last_eat = get_time_milliseconds((philo[i])->tv);
	return (0);
}

int	returner(int ac, t_vars vars)
{
	if (ac != 5 && ac != 6)
	{
		printf("wrong number of args\n");
		return (1);
	}
	if (vars.times_must_eat == 0)
		return (0);
	return (2);
}

int	main(int ac, char **av)
{
	int			i;
	t_philo		**philo;
	t_vars		vars;

	if (parsing(&vars, av, ac) == -1)
		return (returner(ac, vars));
	philo = init_vars(&vars);
	if (philo == NULL)
	{
		printf("allocation failed\n");
		return (3);
	}
	pthread_mutex_lock(&(vars.start_lock));
	if (vars.number_of_philosophers == 1)
		create_lonely_philo(&vars, philo);
	else if (create_threads(&vars, philo) == -1)
		return (exiter(4, philo, vars));
	watching(philo);
	i = 0;
	while (i < vars.number_of_philosophers)
		pthread_join(vars.philos[i++], NULL);
	set_all_finished(philo);
	return (exiter(0, philo, vars));
}
