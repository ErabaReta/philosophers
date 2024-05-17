#include "philo.h"

int is_finished(t_philo *philo)
{
    pthread_mutex_lock(&(philo->vars->state_lock));
    if (philo->vars->simulation_finished == 1)
    {
        pthread_mutex_unlock(&(philo->vars->state_lock));
        return 1;
    }
    pthread_mutex_unlock(&(philo->vars->state_lock));
    return (0);
}

size_t  get_time_milliseconds(t_philo *philo)
{
    struct timeval tv;
    size_t  time;

    pthread_mutex_lock(&(philo->vars->time_lock));
    gettimeofday(&tv, NULL);
    time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    pthread_mutex_unlock(&(philo->vars->time_lock));
    return (time);
}

void    logger(t_philo *philo, char *msg)
{
    pthread_mutex_lock(&(philo->vars->log_lock));
    if (is_finished(philo))
    {
         pthread_mutex_unlock(&(philo->vars->log_lock));
         return ;
    }
    printf("%lu %d %s\n", get_time_milliseconds(philo) - philo->vars->initial_timeval, philo->id + 1, msg);
    pthread_mutex_unlock(&(philo->vars->log_lock));
}

void    thinking(t_philo *philo)
{
    logger(philo, "is thinking");
}

void eating(t_philo *philo)
{
    pthread_mutex_lock(&(philo->time_to_eat_lock));
    pthread_mutex_unlock(&(philo->time_to_eat_lock));
    if (philo->id == philo->vars->number_of_philosophers - 1) {
        pthread_mutex_lock(&philo->vars->forks[0]);
        logger(philo, "has taken a fork");
        pthread_mutex_lock(&philo->vars->forks[philo->id]);
        logger(philo, "has taken a fork");
    } else {
        pthread_mutex_lock(&philo->vars->forks[philo->id]);
        logger(philo, "has taken a fork");
        pthread_mutex_lock(&philo->vars->forks[philo->id + 1]);
        logger(philo, "has taken a fork");
    }
    philo->last_eat = get_time_milliseconds(philo);
    logger(philo, "is eating");
    usleep(philo->vars->time_to_eat * 1000);
    pthread_mutex_unlock(&philo->vars->forks[philo->id]);
    if (philo->id == philo->vars->number_of_philosophers - 1)
        pthread_mutex_unlock(&philo->vars->forks[0]);
    else
        pthread_mutex_unlock(&philo->vars->forks[philo->id + 1]);
}
void    sleeping(t_philo *philo)
{
    logger(philo, "is sleeping");
    usleep(philo->vars->time_to_sleep * 1000);
}


// t_vars *get_vars(void)
// {
//     static t_vars   vars;
//     return  (&vars);
// }

int died(t_philo *philo)
{

    if (get_time_milliseconds(philo) - philo->last_eat >= philo->vars->time_to_die)
    {
        pthread_mutex_lock(&((philo->vars->state_lock)));
        philo->vars->simulation_finished = 1;
        pthread_mutex_unlock(&((philo->vars->state_lock)));
        pthread_mutex_lock(&(philo->vars->log_lock));
        printf("%lu %d died\n", get_time_milliseconds(philo) - philo->vars->initial_timeval, philo->id + 1);
        pthread_mutex_unlock(&(philo->vars->log_lock));
        return (1);
    }
    return (0);
}

int	time_to_eat(t_philo *philo)
{
	size_t	current_time;
	size_t	turn;

	current_time = get_time_milliseconds(philo);
	if ((philo->vars->number_of_philosophers % 2) == 0)
	{
		turn = ((current_time - philo->vars->initial_timeval) / (philo->vars->time_to_eat)) % 2;
		if ((turn == 0) && (philo->id % 2 == 0))
			return (1);
		else if ((turn == 1) && (philo->id % 2 == 1))
			return (1);
		else
			return (0);
	}
	else
	{
		turn = ((current_time - philo->vars->initial_timeval) / (philo->vars->time_to_eat)) % 3;

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
		{
			return (0);
		}
	}
}

void    *watching(void *ptr)
{
		t_philo	*philo;

	philo = (t_philo *)ptr;
	// printf("%d is watching\n", philo->id + 1);
    pthread_mutex_lock(&(philo->time_to_eat_lock));


	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));


    while (!is_finished(philo) && !died(philo))
    {
		while ( !is_finished(philo) && !died(philo) )
		{
			if(time_to_eat(philo))
			{
				pthread_mutex_unlock(&(philo->time_to_eat_lock));
				// printf("turn %lu its time for %d to eat\n", ((get_time_milliseconds(philo) - philo->vars->initial_timeval) / (philo->vars->time_to_eat)) % 3, philo->id + 1);
				break ;
			}
		}
		while (!is_finished(philo) && !died(philo))
		{
			if(!time_to_eat(philo))
			{
    			pthread_mutex_lock(&(philo->time_to_eat_lock));
				// printf("turn %lu its time for %d to stop eating\n",((get_time_milliseconds(philo) - philo->vars->initial_timeval) / (philo->vars->time_to_eat)) % 3, philo->id + 1);
				break ;
			}
		}
    }
    return NULL;
}

void    *routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	// exit(0);


	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));

    while (!is_finished(philo))
    {

        thinking(philo);
        eating(philo);
        sleeping(philo);

    }
    return philo;
}

t_philo **init_vars(char **av, int ac, t_vars *vars)
{
    int i;

    vars->number_of_philosophers = ft_atoi(av[1]);
    vars->time_to_die = ft_atoi(av[2]);
    vars->time_to_eat = ft_atoi(av[3]);
    vars->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        vars->umber_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    else
        vars->umber_of_times_each_philosopher_must_eat = -1;
    vars->philos = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers);
    vars->watchers = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers);
    vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers); 
    pthread_mutex_init(&(vars->state_lock), NULL);
    pthread_mutex_init(&(vars->log_lock), NULL);
    pthread_mutex_init(&(vars->time_lock), NULL);
    pthread_mutex_init(&(vars->start_lock), NULL);
    t_philo **philo = (t_philo **)malloc(sizeof(t_philo *) * vars->number_of_philosophers);
    i = 0;
    while (i < vars->number_of_philosophers)
    {
        philo[i] = (t_philo *)malloc(sizeof(t_philo) * 1);
		philo[i]->vars = vars;
        philo[i]->id = i;
        pthread_mutex_init(&(philo[i]->watcher_lock), NULL);
        pthread_mutex_init(&(philo[i]->time_to_eat_lock), NULL);
        // philo[i]->last_eat = get_time_milliseconds(philo[i]);//philo->vars->initial_timeval;
        pthread_mutex_init(&(vars->forks[i]), NULL);
        i++;
    }

    // vars->initial_timeval = get_time_milliseconds(*philo);
    vars->simulation_finished = 0;
    return (philo);
}

int main(int ac, char **av)
{
    int i;
    t_philo **philo;
    t_philo *joined;
	t_vars vars;

    if (ac != 5 && ac != 6)
    {
        printf("wrong args\n");
        return (1);
    }
    philo = init_vars(av, ac, &vars);
    i = 0;

	pthread_mutex_lock(&(vars.start_lock));

    while (i < vars.number_of_philosophers)
    {
		if (pthread_create(&(vars.watchers[(philo[i])->id]), NULL, watching, philo[i]) != 0)
   		 {
        	printf("failed to create watcher thread\n");
        	exiter(1, *philo);
    	}

    	pthread_detach(vars.watchers[(philo[i])->id]);

        if (pthread_create(&(vars.philos[i]), NULL, routine, philo[i]) != 0)
        {
            printf("failed to create philo thread\n");
            exiter(1, *philo);
        }
        i++;
    }
	i = 0;

	vars.initial_timeval = get_time_milliseconds(*philo);
	while (i < vars.number_of_philosophers)
	{
		philo[i]->last_eat = get_time_milliseconds(philo[i]);
		i++;
	}
	
	pthread_mutex_unlock(&(vars.start_lock));

    i = 0;
    while (i < vars.number_of_philosophers)
    {
        if (pthread_join(vars.philos[i], (void **)&joined) != 0)
        {
            printf("failed to join thread %d\n", i);
            continue ;
        }
            pthread_mutex_destroy(&(joined->watcher_lock));
            free(joined);
        i++;
    }
    exiter(0, *philo);

    return 0;
}