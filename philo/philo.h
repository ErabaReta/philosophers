/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:27 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/08 21:17:36 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
//===================================================================

typedef struct s_vars
{
	long			number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat ;
	size_t			time_to_sleep ;
	long			times_must_eat;
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	start_lock;
	size_t			initial_timeval;
	struct timeval	tv;
}	t_vars;
//===================================================================
typedef struct s_philo
{
	int				id;
	char			think_logged;
	pthread_mutex_t	think_lock;
	size_t			last_eat;
	pthread_mutex_t	last_eat_lock;
	char			eat_logged;
	pthread_mutex_t	eat_lock;
	char			sleep_logged;
	pthread_mutex_t	sleep_lock;
	char			fork_logged;
	pthread_mutex_t	fork_lock;
	int				its_time_to_eat;
	struct timeval	tv;
	char			simulation_finished;
	pthread_mutex_t	state_lock;
	int				count_meals;
	t_vars			*vars;
}	t_philo;

//====================================================

size_t	get_time_milliseconds(struct timeval tv);
int		is_finished(t_philo *philo);
int		all_is_finished(t_philo **philo);
//====================================================

long	ft_philo_atoi(const char *str);
void	*routine(void *ptr);
t_philo	**init_vars(t_vars *vars);
int		exiter(int err, t_philo **philo, t_vars vars);
//====================================================

void	*watching(void *ptr);
int		time_to_eat(t_philo *philo);
void	set_all_finished(t_philo **philo);
//===== loggers ======================================

int		logger(t_philo *philo, char *msg);
int		have_tought(t_philo *philo);
int		had_fork(t_philo *philo);
int		have_ate(t_philo *philo);
int		have_slept(t_philo *philo);
//=====================================================
int		parsing(t_vars *vars, char **av, int ac);
void	eating(t_philo *philo);
//===
int		create_lonely_philo(t_vars *vars, t_philo **philo);
//===
void	wait_for(t_philo *philo, size_t time);

#endif