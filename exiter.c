/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:41 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/24 20:49:28 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	exiter(int err, t_philo **philo, t_vars vars)
{
	int	i;

	printf("exiting ..\n");
	// pthread_mutex_lock(&(vars.state_lock));
	i = 0;
	while ( i < vars.number_of_philosophers)
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
	pthread_mutex_destroy(&(vars.time_lock));
	// pthread_mutex_unlock(&(vars.state_lock));
	// pthread_mutex_destroy(&(vars.state_lock));
	return (err);
}