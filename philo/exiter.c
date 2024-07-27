/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:41 by eouhrich          #+#    #+#             */
/*   Updated: 2024/06/11 14:08:02 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	exiter(int err, t_philo **philo, t_vars vars)
{
	int	i;

	i = 0;
	while (i < vars.number_of_philosophers)
	{
		pthread_mutex_destroy(&(philo[i]->last_eat_lock));
		pthread_mutex_destroy(&(vars.forks[i]));
		pthread_mutex_destroy(&(philo[i]->eat_lock));
		pthread_mutex_destroy(&(philo[i]->fork_lock));
		pthread_mutex_destroy(&(philo[i]->sleep_lock));
		pthread_mutex_destroy(&(philo[i]->think_lock));
		pthread_mutex_destroy(&(philo[i]->state_lock));
		free(philo[i]);
		i++;
	}
	free(philo);
	free(vars.forks);
	free(vars.philos);
	pthread_mutex_destroy(&(vars.start_lock));
	return (err);
}
