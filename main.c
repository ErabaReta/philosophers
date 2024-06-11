/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:33 by eouhrich          #+#    #+#             */
/*   Updated: 2024/06/11 14:03:31 by eouhrich         ###   ########.fr       */
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

int	create_threads(t_vars *vars, t_philo **philo, pthread_t	*watcher)
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
	i = 0;
	if (pthread_create(watcher, NULL, watching, philo) != 0)
	{
		printf("failed to create watcher thread\n");
		return (-1);
	}
	i = 0;
	usleep(5000);
	vars->initial_timeval = get_time_milliseconds((*philo)->tv);
	while (i < vars->number_of_philosophers)
		philo[i++]->last_eat = get_time_milliseconds((*philo)->tv);
	return (0);
}

int	main(int ac, char **av)
{
	int			i;
	t_philo		**philo;
	t_vars		vars;
	pthread_t	watcher;

	if ((ac != 5 && ac != 6) || parsing(&vars, av, ac) == -1)
		return (1);
	philo = init_vars(&vars);
	if (philo == NULL)
	{
		printf("allocation failed\n");
		return (2);
	}
	pthread_mutex_lock(&(vars.start_lock));
	if (create_threads(&vars, philo, &watcher) == -1)
		return (exiter(3, philo, vars));
	pthread_mutex_unlock(&(vars.start_lock));
	i = 0;
	while (i < vars.number_of_philosophers)
		pthread_join(vars.philos[i++], NULL);
	set_all_finished(philo);
	pthread_join(watcher, NULL);
	return (exiter(0, philo, vars));
}
