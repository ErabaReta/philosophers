/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:02 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/05 00:50:31 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_all_finished(t_philo **philo)
{
	int	i;

	i = 0;
	while (i < (*philo)->vars->number_of_philosophers)
	{
		pthread_mutex_lock(&((philo[i]->state_lock)));
		philo[i]->simulation_finished = 1;
		pthread_mutex_unlock(&((philo[i]->state_lock)));
		i++;
	}
}

int	all_is_finished(t_philo **philo)
{
	int	i;

	i = 0;
	while (i < (*philo)->vars->number_of_philosophers)
	{
		pthread_mutex_lock(&((philo[i]->state_lock)));
		if (philo[i]->simulation_finished == 0)
		{
			pthread_mutex_unlock(&((philo[i]->state_lock)));
			return (0);
		}
		pthread_mutex_unlock(&((philo[i]->state_lock)));
		i++;
	}
	return (1);
}

int	died(t_philo **philo, int i)
{
	pthread_mutex_lock(&(philo[i]->last_eat_lock));
	if (get_time_milliseconds(philo[i]->vars->tv)
		- philo[i]->last_eat > philo[i]->vars->time_to_die)
	{
		pthread_mutex_unlock(&(philo[i]->last_eat_lock));
		printf("%lu %d died\n", get_time_milliseconds(philo[i]->vars->tv)
			- philo[i]->vars->initial_timeval, philo[i]->id + 1);
		set_all_finished(philo);
		return (1);
	}
	pthread_mutex_unlock(&(philo[i]->last_eat_lock));
	return (0);
}

int	all_ate(t_philo **philo)
{
	int	n_philos;
	int	i;

	n_philos = (*philo)->vars->number_of_philosophers;
	i = 0;
	while (i < n_philos)
	{
		pthread_mutex_lock(&(philo[i]->last_eat_lock));
		if (philo[i]->count_meals < philo[i]->vars->times_must_eat)
		{
			pthread_mutex_unlock(&(philo[i]->last_eat_lock));
			return (0);
		}
		pthread_mutex_unlock(&(philo[i]->last_eat_lock));
		i++;
	}
	return (1);
}

void	*watching(void *ptr)
{
	t_philo	**philo;
	int		i;

	philo = (t_philo **)ptr;
	pthread_mutex_lock(&((*philo)->vars->start_lock));
	pthread_mutex_unlock(&((*philo)->vars->start_lock));
	i = 0;
	while (!all_is_finished(philo))
	{
		if (died(philo, i))
			break ;
		if (have_tought(philo[i]))
			logger(philo[i], "is thinking");
		else if (had_fork(philo[i]))
			logger(philo[i], "has taken a fork");
		else if (have_ate(philo[i]))
		{
			logger(philo[i], "is eating");
			if ((*philo)->vars->times_must_eat != -1 && all_ate(philo))
			{
				printf("all ate\n");
				set_all_finished(philo);
				break ;
			}
		}
		else if (have_slept(philo[i]))
			logger(philo[i], "is sleeping");
		if (++i == (*philo)->vars->number_of_philosophers)
			i = 0;
	}
	return (NULL);
}
