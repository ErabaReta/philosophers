/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logged_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:24:52 by eouhrich          #+#    #+#             */
/*   Updated: 2024/06/10 22:36:10 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	logger(t_philo *philo, char *msg)
{
	if (is_finished(philo))
	{
		return ;
	}
	printf("%lu %d %s\n", get_time_milliseconds(philo->tv)
		- philo->vars->initial_timeval, philo->id + 1, msg);
}

int	have_tought(t_philo *philo)
{
	pthread_mutex_lock(&(philo->think_lock));
	if (philo->think_logged == 0)
	{
		philo->think_logged = 1;
		pthread_mutex_unlock(&(philo->think_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo->think_lock));
	return (0);
}

int	had_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_lock));
	if (philo->fork_logged == 0 || philo->fork_logged == -1)
	{
		philo->fork_logged += 1;
		pthread_mutex_unlock(&(philo->fork_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo->fork_lock));
	return (0);
}

int	have_ate(t_philo *philo)
{
	pthread_mutex_lock(&(philo->eat_lock));
	if (philo->eat_logged == 0)
	{
		philo->eat_logged = 1;
		pthread_mutex_unlock(&(philo->eat_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo->eat_lock));
	return (0);
}

int	have_slept(t_philo *philo)
{
	pthread_mutex_lock(&(philo->sleep_lock));
	if (philo->sleep_logged == 0)
	{
		philo->sleep_logged = 1;
		pthread_mutex_unlock(&(philo->sleep_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo->sleep_lock));
	return (0);
}
