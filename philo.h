#ifndef PHILO_H
# define PHILO_H

// # define number_of_philosophers

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct	s_vars
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat ;
	int time_to_sleep ;
	int umber_of_times_each_philosopher_must_eat;
	pthread_t		*philo;
	pthread_mutex_t	*forks;
	struct timeval	initial_timeval;
}	t_vars;



//=
long	ft_atoi(const char *str);
#endif