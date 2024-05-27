/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:00:18 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/25 21:44:18 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    logger(t_philo *philo, char *msg)
{
    pthread_mutex_lock(&(philo->vars->log_lock));
    if (is_finished(philo))
    {
		// printf(" %d cant log\n", philo->id + 1);

         pthread_mutex_unlock(&(philo->vars->log_lock));
         return ;
    }
    printf("%lu %d %s\n", get_time_milliseconds(philo) - philo->vars->initial_timeval, philo->id + 1, msg);
	
	// ft_putnbr(get_time_milliseconds(philo) - philo->vars->initial_timeval);
	// ft_putchar(' ');
	// ft_putnbr(philo->id + 1);
	// ft_putchar(' ');
	// print_str(msg);
	// ft_putchar('\n');
    pthread_mutex_unlock(&(philo->vars->log_lock));
}

int	have_tought(t_philo *philo)///////
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

void    *noting(void *ptr)
{
	t_philo **philo;
	int	i;

	philo = (t_philo **)ptr;
	// printf("noting...\n");///
	pthread_mutex_lock(&((*philo)->vars->start_lock));
	pthread_mutex_unlock(&((*philo)->vars->start_lock));
	i = 0;
	while (!all_is_finished((philo)))
	{
		if (have_tought(philo[i]))
		{
			logger(philo[i], "is thinking");
		}
		else if (had_fork(philo[i]))
		{
			logger(philo[i], "has taken a fork");
		}
		// if (had_fork(philo[i]))
		// {
		// 	logger(philo[i], "has taken a fork");
		// }
		else if (have_ate(philo[i]))
		{
			logger(philo[i], "is eating");
		}
		else if (have_slept(philo[i]))
		{
			logger(philo[i], "is sleeping");
		}
		i++;
		if ( i == (*philo)->vars->number_of_philosophers)
			i = 0;
		// usleep(150);
		// printf("noting %d philo's actions\n", philo[i]->id);//
	}
	return (NULL);
}