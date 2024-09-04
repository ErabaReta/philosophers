/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:57:13 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/05 00:48:45 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_whose_turn(t_philo *philo)
{
	size_t	current_time;
	size_t	turn;
	size_t	range;

	current_time = get_time_milliseconds(philo->tv)
		- philo->vars->initial_timeval;
	if ((philo->vars->time_to_eat * 2) < philo->vars->time_to_sleep)
	{
		range = philo->vars->time_to_eat + philo->vars->time_to_sleep;
		turn = (current_time % range);
		if (turn < philo->vars->time_to_eat)
			turn = 0;
		else if (philo->vars->time_to_eat <= turn
			&& turn < (philo->vars->time_to_eat * 2))
			turn = 1;
		else if ((philo->vars->time_to_eat * 2) <= turn
			&& turn < (philo->vars->time_to_eat * 3))
			turn = 2;
		else
			turn = 3;
	}
	else
		turn = (current_time / philo->vars->time_to_eat) % 3;
	return (turn);
}

int	time_to_eat(t_philo *philo)
{
	size_t	turn;

	turn = get_whose_turn(philo);
	if ((turn == 0) && (philo->id % 2 == 0)
		&& (philo->id != (philo->vars->number_of_philosophers - 1)))
		return (1);
	else if ((turn == 1) && (philo->id % 2 == 1))
		return (1);
	else if ((turn == 2) && (philo->id == (philo->vars->number_of_philosophers
				- 1)))
		return (1);
	else
		return (0);
}

void	taking_forks(t_philo *philo)
{
	if (philo->id == philo->vars->number_of_philosophers - 1)
	{
		pthread_mutex_lock(&philo->vars->forks[0]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged = 0;
		pthread_mutex_unlock(&(philo->fork_lock));
		pthread_mutex_lock(&philo->vars->forks[philo->id]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
	}
	else
	{
		pthread_mutex_lock(&philo->vars->forks[philo->id]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged = 0;
		pthread_mutex_unlock(&(philo->fork_lock));
		pthread_mutex_lock(&philo->vars->forks[philo->id + 1]);
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
	}
}

void	eating(t_philo *philo)
{
	while ((philo->vars->number_of_philosophers % 2) == 1
		&& !time_to_eat(philo) && !is_finished(philo))
		wait_for(philo, 1);
	taking_forks(philo);
	pthread_mutex_lock(&(philo->eat_lock));
	philo->eat_logged = 0;
	pthread_mutex_unlock(&(philo->eat_lock));
	pthread_mutex_lock(&(philo->last_eat_lock));
	philo->last_eat = get_time_milliseconds(philo->tv);
	philo->count_meals++;
	pthread_mutex_unlock(&(philo->last_eat_lock));
	wait_for(philo, philo->vars->time_to_eat);
	pthread_mutex_unlock(&philo->vars->forks[philo->id]);
	if (philo->id == philo->vars->number_of_philosophers - 1)
		pthread_mutex_unlock(&philo->vars->forks[0]);
	else
		pthread_mutex_unlock(&philo->vars->forks[philo->id + 1]);
}
