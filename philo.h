/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:27 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/29 11:37:16 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// # define number_of_philosophers

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>
//===================================================================

typedef struct	s_vars
{
	int	number_of_philosophers;
	size_t	time_to_die;
	size_t	time_to_eat ;
	size_t time_to_sleep ;
	int umber_of_times_each_philosopher_must_eat;
	int	finishers_count;
	pthread_mutex_t	simulation_started;
	// pthread_mutex_t	state_lock;
	pthread_t		*philos;
	pthread_t		*watchers;
	pthread_mutex_t	*forks;
	pthread_mutex_t log_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	start_lock;
	size_t	initial_timeval;
	struct timeval	tv;
}	t_vars;
//===================================================================
typedef struct	s_philo
{
	int		id;
	size_t	last_think;
	char	think_logged;
	pthread_mutex_t	think_lock;
	size_t	last_eat;
	pthread_mutex_t	last_eat_lock;
	
	char	eat_logged;
	pthread_mutex_t	eat_lock;

	char	sleep_logged;
	pthread_mutex_t	sleep_lock;

	char	fork_logged;
	pthread_mutex_t	fork_lock;

	pthread_mutex_t	watcher_lock;
	pthread_mutex_t	time_to_eat_lock;
	int				its_time_to_eat;

	struct timeval	tv;
	char	simulation_finished;
	pthread_mutex_t	state_lock;
	int	count_meals;
	t_vars			*vars;
}	t_philo;

//=
size_t  get_time_milliseconds(	struct timeval	tv);
int is_finished(t_philo *philo);
int	all_is_finished(t_philo **philo);
//=
long	ft_atoi(const char *str);
// t_vars *get_vars(void);
int	exiter(int err, t_philo **philo, t_vars vars);
//=
void    *watching(void *ptr);
int	time_to_eat(t_philo *philo);
void	set_all_finished(t_philo **philo);
//=
void    *noting(void *ptr);
void    logger(t_philo *philo, char *msg);
///=====
void	ft_putnbr(int n);
void	print_str(char *str);
void	ft_putchar(char c);
#endif