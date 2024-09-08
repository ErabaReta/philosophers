/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:57:13 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:22:44 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	taking_forks(t_philo *philo)
{
	if (philo->id == philo->vars->number_of_philosophers)
	{
		pthread_mutex_lock(&philo->vars->forks[0]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged = 0;
		pthread_mutex_unlock(&(philo->fork_lock));
		pthread_mutex_lock(&philo->vars->forks[philo->id - 1]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
	}
	else
	{
		pthread_mutex_lock(&philo->vars->forks[philo->id - 1]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged = 0;
		pthread_mutex_unlock(&(philo->fork_lock));
		pthread_mutex_lock(&philo->vars->forks[philo->id]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
	}
}

void	eating(t_philo *philo)
{
	taking_forks(philo);
	pthread_mutex_lock(&(philo->eat_lock));
	philo->eat_logged = 0;
	pthread_mutex_unlock(&(philo->eat_lock));
	pthread_mutex_lock(&(philo->last_eat_lock));
	philo->last_eat = get_time_milliseconds(philo->tv);
	philo->count_meals++;
	pthread_mutex_unlock(&(philo->last_eat_lock));
	wait_for(philo, philo->vars->time_to_eat);
	pthread_mutex_unlock(&philo->vars->forks[philo->id - 1]);
	if (philo->id == philo->vars->number_of_philosophers)
		pthread_mutex_unlock(&philo->vars->forks[0]);
	else
		pthread_mutex_unlock(&philo->vars->forks[philo->id]);
}
