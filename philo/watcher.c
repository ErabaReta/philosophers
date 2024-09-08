/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:02 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:31:53 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	died(t_philo **philo, int i)
{
	pthread_mutex_lock(&(philo[i]->last_eat_lock));
	if (get_time_milliseconds(philo[i]->vars->tv)
		- philo[i]->last_eat > philo[i]->vars->time_to_die)
	{
		pthread_mutex_unlock(&(philo[i]->last_eat_lock));
		printf("%lu %d died\n", get_time_milliseconds(philo[i]->vars->tv)
			- philo[i]->vars->initial_timeval, philo[i]->id);
		set_all_finished(philo);
		return (1);
	}
	pthread_mutex_unlock(&(philo[i]->last_eat_lock));
	return (0);
}

int	last_group_ate(t_philo **philo, int n_philos)
{
	int	i;

	i = n_philos - 1;
	while (i >= 0)
	{
		pthread_mutex_lock(&(philo[i]->last_eat_lock));
		if ((philo[i]->count_meals < philo[i]->vars->times_must_eat)
			|| have_ate(philo[i]))
		{
			pthread_mutex_unlock(&(philo[i]->last_eat_lock));
			return (0);
		}
		pthread_mutex_unlock(&(philo[i]->last_eat_lock));
		i -= 2;
	}
	set_all_finished(philo);
	return (1);
}

int	all_ate(t_philo **philo)
{
	int	n_philos;

	if ((*philo)->vars->times_must_eat == -1)
		return (0);
	n_philos = (*philo)->vars->number_of_philosophers;
	if (n_philos % 2 == 1)
	{
		pthread_mutex_lock(&(philo[n_philos - 1]->last_eat_lock));
		if (philo[n_philos - 1]->count_meals
			>= (*philo)->vars->times_must_eat
			&& !have_ate(philo[n_philos -1]))
		{
			pthread_mutex_unlock(&(philo[n_philos - 1]->last_eat_lock));
			set_all_finished(philo);
			return (1);
		}
		else
		{
			pthread_mutex_unlock(&(philo[n_philos - 1]->last_eat_lock));
			return (0);
		}
	}
	else
		return (last_group_ate(philo, n_philos));
}

void	log_actions(t_philo *philo)
{
	if (have_tought(philo))
		logger(philo, "is thinking");
	else if (had_fork(philo))
		logger(philo, "has taken a fork");
	else if (have_ate(philo))
	{
		logger(philo, "is eating");
		pthread_mutex_lock(&(philo->eat_lock));
		philo->eat_logged = 1;
		pthread_mutex_unlock(&(philo->eat_lock));
	}
	else if (have_slept(philo))
		logger(philo, "is sleeping");
}

void	*watching(void *ptr)
{
	t_philo	**philo;
	int		i;

	philo = (t_philo **)ptr;
	pthread_mutex_unlock(&((*philo)->vars->start_lock));
	(*philo)->vars->initial_timeval = get_time_milliseconds((*philo)->tv);
	i = 0;
	while (!all_is_finished(philo))
	{
		if (died(philo, i) || all_ate(philo))
			break ;
		log_actions(philo[i]);
		if (++i == (*philo)->vars->number_of_philosophers)
			i = 0;
	}
	return (NULL);
}
