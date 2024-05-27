/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:02 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/27 13:16:55 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	time_to_eat(t_philo *philo)
{
	size_t	current_time;
	size_t	turn;

	current_time = get_time_milliseconds(philo) - philo->vars->initial_timeval;
	// if ((philo->vars->number_of_philosophers % 2) == 0)
	// {
	// 	turn = ((current_time) / (philo->vars->time_to_eat)) % 2;
	// 	if ((turn == 0) && (philo->id % 2 == 0))
	// 		return (1);
	// 	else if ((turn == 1) && (philo->id % 2 == 1))
	// 		return (1);
	// 	else
	// 		return (0);
	// }
	// else
	// {
		turn = (current_time / (philo->vars->time_to_eat)) % 3;

		if ((turn == 0) && (philo->id % 2 == 0) && (philo->id != (philo->vars->number_of_philosophers - 1)))
		{
			return (1);
		}
		else if ((turn == 1) && (philo->id % 2 == 1))
		{
			return (1);
		}
		else if ((turn == 2) && (philo->id == (philo->vars->number_of_philosophers - 1)))
		{
			return (1);
		}
		else
			return (0);
	// }
}

void	set_all_finished(t_philo **philo)
{
	int i;

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

int died(t_philo **philo, int i)
{
	pthread_mutex_lock(&(philo[i]->last_eat_lock));
	if (philo[i]->count_meals == philo[i]->vars->umber_of_times_each_philosopher_must_eat)
		return (0);
	if (get_time_milliseconds(philo[i]) - philo[i]->last_eat > philo[i]->vars->time_to_die)//////////////////////////////////
	{
		pthread_mutex_unlock(&(philo[i]->last_eat_lock));
		// printf("%lu %d died\n", get_time_milliseconds(philo[i]) - philo[i]->vars->initial_timeval, philo[i]->id + 1);

		set_all_finished(philo);
		// pthread_mutex_lock(&((philo->vars->state_lock)));
		// philo->simulation_finished = 1;
		// pthread_mutex_unlock(&((philo->vars->state_lock)));
		pthread_mutex_lock(&(philo[i]->vars->log_lock));
		printf("%lu %d died\n", get_time_milliseconds(philo[i]) - philo[i]->vars->initial_timeval, philo[i]->id + 1);
		pthread_mutex_unlock(&(philo[i]->vars->log_lock));
		return (1);
	}
	pthread_mutex_unlock(&(philo[i]->last_eat_lock));
    return (0);
}


// int	all_have_eat(t_philo **philo)
// {
// 	int	i;

// 	i = 0;
// 	while (i < (*philo)->vars->number_of_philosophers)
// 	{
// 		// pthread_mutex_lock(&((philo[i]->state_lock)));
// 		if (philo[i]->count_meals == philo[i])
// 		{
// 				// pthread_mutex_unlock(&((philo[i]->state_lock)));
// 			return (0);
// 		}
// 		// pthread_mutex_unlock(&((philo[i]->state_lock)));
// 		i++;
// 	}
// 	return (1);
// }

void    *watching(void *ptr)
{
	t_philo	**philo;
	int i;

	philo = (t_philo **)ptr;
	// printf("%d is watching\n", philo->id + 1);

	// printf("turn %lu its time for %d to stop eating\n",((get_time_milliseconds(philo) - philo->vars->initial_timeval) / (philo->vars->time_to_eat)) % 3, philo->id + 1);
	i = 0;
	// while (i < (*philo)->vars->number_of_philosophers)
	// {
    // 	pthread_mutex_lock(&((philo[i])->time_to_eat_lock));
	// 	philo[i]->its_time_to_eat = 0;
	// 	i++;
	// }



	pthread_mutex_lock(&((*philo)->vars->start_lock));
	pthread_mutex_unlock(&((*philo)->vars->start_lock));

		i = 0;
		while ( !all_is_finished(philo))
		{
			printf("watching %d\n", philo[i]->id + 1);
			if (died(philo, i))
			{
				// set_all_finished(philo);	
				break ;
			}
			// usleep(200);
			// if (philo[i]->its_time_to_eat == 0 && time_to_eat(philo[i]))
			// {
			// 	philo[i]->its_time_to_eat = 1;

			// 	pthread_mutex_unlock(&((philo[i])->time_to_eat_lock));
			// 	printf("turn %lu its time for %d to eat\n", ((get_time_milliseconds(*philo) - (*philo)->vars->initial_timeval) / ((philo[i])->vars->time_to_eat)) % 3, (philo[i])->id + 1);
			// }
			// else if(philo[i]->its_time_to_eat == 1 && !time_to_eat((philo[i])))
			// {
			// 	philo[i]->its_time_to_eat = 0;
    		// 	pthread_mutex_lock(&((philo[i])->time_to_eat_lock));
			// 	printf("turn %lu its time for %d to stop eating\n",((get_time_milliseconds(*philo) - (*philo)->vars->initial_timeval) / ((philo[i])->vars->time_to_eat)) % 3, (philo[i])->id + 1);
			// }
			i++;
			if ( i == (*philo)->vars->number_of_philosophers)
				i = 0;
		}

    return NULL;
}