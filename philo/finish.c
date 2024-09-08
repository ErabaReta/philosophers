/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:25:34 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:25:51 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

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
