/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 21:10:04 by eouhrich          #+#    #+#             */
/*   Updated: 2024/06/10 22:40:03 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos_locks(t_philo **philo, t_vars *vars, int i)
{
	pthread_mutex_init(&(philo[i]->eat_lock), NULL);
	pthread_mutex_init(&(philo[i]->think_lock), NULL);
	pthread_mutex_init(&(philo[i]->sleep_lock), NULL);
	pthread_mutex_init(&(philo[i]->fork_lock), NULL);
	pthread_mutex_init(&(philo[i]->last_eat_lock), NULL);
	pthread_mutex_init(&(philo[i]->state_lock), NULL);
	pthread_mutex_init(&(vars->forks[i]), NULL);
}

t_philo	**init_philos(t_philo **philo, t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->number_of_philosophers)
	{
		philo[i] = (t_philo *)malloc(sizeof(t_philo) * 1);
		if (philo[i] == NULL)
		{
			vars->number_of_philosophers = i + 1;
			exiter(0, philo, *vars);
			return (NULL);
		}
		philo[i]->vars = vars;
		philo[i]->id = i;
		init_philos_locks(philo, vars, i);
		philo[i]->think_logged = 1;
		philo[i]->fork_logged = 1;
		philo[i]->eat_logged = 1;
		philo[i]->sleep_logged = 1;
		philo[i]->simulation_finished = 0;
		philo[i]->count_meals = 0;
		i++;
	}
	return (philo);
}

t_philo	**init_vars(t_vars *vars)
{
	t_philo	**philo;

	vars->philos = (pthread_t *)malloc(sizeof(pthread_t)
			* vars->number_of_philosophers);
	if (vars->philos == NULL)
		return (NULL);
	vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* vars->number_of_philosophers);
	if (vars->forks == NULL)
	{
		free(vars->philos);
		return (NULL);
	}
	pthread_mutex_init(&(vars->start_lock), NULL);
	philo = (t_philo **)malloc(sizeof(t_philo *)
			* vars->number_of_philosophers);
	return (init_philos(philo, vars));
}
