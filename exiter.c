#include "philo.h"

int	exiter(int err, t_philo *philo)
{
	int	i;

	printf("exiting ..\n");
	pthread_mutex_lock(&(philo->vars->state_lock));
	i = 0;
	while ( i < philo->vars->number_of_philosophers)
	{
		pthread_mutex_destroy(&(philo->vars->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(philo->vars->time_lock));
	pthread_mutex_unlock(&(philo->vars->state_lock));
	pthread_mutex_destroy(&(philo->vars->state_lock));
	return (err);
}