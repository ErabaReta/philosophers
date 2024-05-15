#include "philo.h"

void	exiter(int err)
{
	int	i;

	printf("exiting ..\n");
	pthread_mutex_lock(&(get_vars()->state_lock));
	i = 0;
	while ( i < get_vars()->number_of_philosophers)
	{
		pthread_mutex_destroy(&(get_vars()->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(get_vars()->time_lock));
	pthread_mutex_unlock(&(get_vars()->state_lock));
	pthread_mutex_destroy(&(get_vars()->state_lock));
	exit(err);
}