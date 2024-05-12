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
	pthread_t		*philos;
	pthread_t		*watchers;
	pthread_mutex_t	*forks;
	pthread_mutex_t log_lock;
	size_t	initial_timeval;
}	t_vars;
//===================================================================
typedef struct	s_philo
{
	int		id;
	pthread_mutex_t	watcher_lock;
	size_t last_meal; 
}	t_philo;


//=
long	ft_atoi(const char *str);
t_vars *get_vars(void);
void	exiter();
#endif