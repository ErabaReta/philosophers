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
	char	simulation_finished;
	pthread_mutex_t	simulation_started;
	pthread_mutex_t	state_lock;
	pthread_t		*philos;
	pthread_t		*watchers;
	pthread_mutex_t	*forks;
	pthread_mutex_t log_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	start_lock;
	size_t	initial_timeval;
}	t_vars;
//===================================================================
typedef struct	s_philo
{
	int		id;
	size_t	last_think;
	char	think_logged;
	pthread_mutex_t	think_lock;
	size_t	last_eat;
	char	eat_logged;
	pthread_mutex_t	eat_lock;
	size_t	last_sleep;
	char	sleep_logged;
	pthread_mutex_t	sleep_lock;
	size_t	last_left_fork;
	char	left_fork_logged;
	pthread_mutex_t	left_fork_lock;
	size_t	last_right_fork;
	char	right_fork_logged;
	pthread_mutex_t	right_fork_lock;
	pthread_mutex_t	watcher_lock;
	pthread_mutex_t	time_to_eat_lock;
	t_vars			*vars;
}	t_philo;


//=
long	ft_atoi(const char *str);
// t_vars *get_vars(void);
void	exiter(int err, t_philo *philo);

#endif