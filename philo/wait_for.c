/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 18:55:56 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 16:32:26 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for(t_philo *philo, size_t time)
{
	size_t	start;

	start = get_time_milliseconds(philo->tv);
	if (time < 2000)
		usleep((time * 1000) / 2);
	while ((get_time_milliseconds(philo->tv) - start) < time)
	{
		pthread_mutex_lock(&((philo->state_lock)));
		if (philo->simulation_finished)
		{
			pthread_mutex_unlock(&((philo->state_lock)));
			break ;
		}
		pthread_mutex_unlock(&((philo->state_lock)));
		usleep(1);
	}
}
