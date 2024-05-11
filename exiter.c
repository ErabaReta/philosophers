#include "philo.h"

void	exiter(int err)
{
	int	i;
	t_vars *vars;

	printf("exiting ..\n");
	vars = get_vars();
	i = 0;
	while ( i < vars->number_of_philosophers)
	{
		pthread_mutex_destroy(&(vars->forks[i]));
		i++;
	}
	exit(err);
}