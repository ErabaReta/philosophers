/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:33 by eouhrich          #+#    #+#             */
/*   Updated: 2024/05/27 13:15:11 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_finished(t_philo *philo)
{
	// if (philo->count_meals == philo->vars->umber_of_times_each_philosopher_must_eat)
	// 	return (1);
    pthread_mutex_lock(&(philo->state_lock));
    if (philo->simulation_finished == 1)
    {
        pthread_mutex_unlock(&(philo->state_lock));
        return 1;
    }
    pthread_mutex_unlock(&(philo->state_lock));
    return (0);
}

size_t  get_time_milliseconds(t_philo *philo)
{
    gettimeofday(&(philo->tv), NULL);
	// printf("usec => %ld\n", philo->tv.tv_usec);
	// if (((philo->tv.tv_usec / 100) % 10) >= 500)
    // 	return (philo->tv.tv_sec * 1000 + philo->tv.tv_usec / 1000 + 1);
    return (philo->tv.tv_sec * 1000 + philo->tv.tv_usec / 1000);
}


void    thinking(t_philo *philo)
{
    // logger(philo, "is thinking");
	pthread_mutex_lock(&(philo->think_lock));
	philo->think_logged = 0;
	pthread_mutex_unlock(&(philo->think_lock));

}

void eating(t_philo *philo)
{
    // pthread_mutex_lock(&(philo->time_to_eat_lock));
    // pthread_mutex_unlock(&(philo->time_to_eat_lock));

	//=
	while ((philo->vars->number_of_philosophers % 2) == 1 && !time_to_eat(philo))
	{
		usleep(10);
	}
	// if (philo[i]->its_time_to_eat == 0 && time_to_eat(philo[i]))
	//=
	
    if (philo->id == philo->vars->number_of_philosophers - 1) {
        pthread_mutex_lock(&philo->vars->forks[0]);
        // logger(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged = 0;
		pthread_mutex_unlock(&(philo->fork_lock));

        pthread_mutex_lock(&philo->vars->forks[philo->id]);
        // logger(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
    } else {
        pthread_mutex_lock(&philo->vars->forks[philo->id]);///////////
        // logger(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged= 0;
		pthread_mutex_unlock(&(philo->fork_lock));

        pthread_mutex_lock(&philo->vars->forks[philo->id + 1]);/////////
        // logger(philo, "has taken a fork");
		pthread_mutex_lock(&(philo->fork_lock));
		philo->fork_logged -= 1;
		pthread_mutex_unlock(&(philo->fork_lock));
    }
    // logger(philo, "is eating");
		pthread_mutex_lock(&(philo->eat_lock));
		philo->eat_logged = 0;
		pthread_mutex_unlock(&(philo->eat_lock));
		
	pthread_mutex_lock(&(philo->last_eat_lock));
    philo->last_eat = get_time_milliseconds(philo);
	philo->count_meals++;
	pthread_mutex_unlock(&(philo->last_eat_lock));
		// printf("philo ==> %d  ||| philo->count_meals %d \n",philo->id + 1, philo->count_meals);//
	// if (philo->count_meals == philo->vars->umber_of_times_each_philosopher_must_eat)
	// {
	// 	pthread_mutex_lock(&((philo->state_lock)));
	// 	philo->simulation_finished = 1;
	// 	pthread_mutex_unlock(&((philo->state_lock)));
	// }
    usleep(philo->vars->time_to_eat * 1000);
    pthread_mutex_unlock(&philo->vars->forks[philo->id]);
    if (philo->id == philo->vars->number_of_philosophers - 1)
        pthread_mutex_unlock(&philo->vars->forks[0]);
    else
        pthread_mutex_unlock(&philo->vars->forks[philo->id + 1]);
}
void    sleeping(t_philo *philo)
{
    // logger(philo, "is sleeping");
	pthread_mutex_lock(&(philo->sleep_lock));
	philo->sleep_logged = 0;
	pthread_mutex_unlock(&(philo->sleep_lock));
    usleep(philo->vars->time_to_sleep * 1000);
}


// t_vars *get_vars(void)
// {
//     static t_vars   vars;
//     return  (&vars);
// }

void    *routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	// exit(0);


	pthread_mutex_lock(&(philo->vars->start_lock));
	pthread_mutex_unlock(&(philo->vars->start_lock));

	
	if (((philo->id % 2) == 1) || (philo->id == (philo->vars->number_of_philosophers - 1)))
		usleep(3000);
	// if (philo->id == (philo->vars->number_of_philosophers - 1))
	// 	usleep(1000);
    while (!is_finished(philo))
    {
        thinking(philo);
        eating(philo);
		if (philo->count_meals == philo->vars->umber_of_times_each_philosopher_must_eat)
			break ;
        sleeping(philo);
    }
	// printf("%d left the table\n", philo->id + 1);
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
    // pthread_mutex_init(&(vars->state_lock), NULL);
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

        pthread_mutex_init(&(philo[i]->eat_lock), NULL);
        pthread_mutex_init(&(philo[i]->think_lock), NULL);
        pthread_mutex_init(&(philo[i]->sleep_lock), NULL);
        pthread_mutex_init(&(philo[i]->fork_lock), NULL);

        pthread_mutex_init(&(philo[i]->last_eat_lock), NULL);
		pthread_mutex_init(&(philo[i]->state_lock), NULL);//

        pthread_mutex_init(&(philo[i]->time_to_eat_lock), NULL);
        pthread_mutex_init(&(vars->forks[i]), NULL);
		philo[i]->think_logged = 1;
		philo[i]->fork_logged = 1;
		philo[i]->eat_logged = 1;
		philo[i]->sleep_logged = 1;

	    philo[i]->simulation_finished = 0;
	    philo[i]->count_meals = 0;

        i++;
    }

    // vars->initial_timeval = get_time_milliseconds(*philo);
    // vars->simulation_finished = 0;
    return (philo);
}

int main(int ac, char **av)
{
    int i;
    t_philo **philo;
    // t_philo *joined;
	t_vars vars;
	pthread_t	noter;
	pthread_t		watcher;

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
        if (pthread_create(&(vars.philos[i]), NULL, routine, philo[i]) != 0)
        {
            printf("failed to create philo thread\n");
			return (1);///
        }
        i++;
    }

	i = 0;
	// while (i < vars.number_of_philosophers)
	// {
    // 	pthread_mutex_lock(&((philo[i])->time_to_eat_lock));
	// 	philo[i]->its_time_to_eat = 0;
	// 	i++;
	// }
	//====
	if (pthread_create(&noter, NULL, noting, philo) != 0)
   	{
    	printf("failed to create noter thread\n");
    	return (1);////
    }
    	// pthread_detach(noter);
	//=====
	if (pthread_create(&watcher, NULL, watching, philo) != 0)
   	{
        	printf("failed to create watcher thread\n");
        	return (1);////
    }
    	// pthread_detach(watcher);
	i = 0;
	usleep(5000);
	vars.initial_timeval = get_time_milliseconds(*philo);
	while (i < vars.number_of_philosophers)
	{
		philo[i]->last_eat = get_time_milliseconds(*philo);
		i++;
	}
	pthread_mutex_unlock(&(vars.start_lock));

    i = 0;
    while (i < vars.number_of_philosophers)
    {
        if (pthread_join(vars.philos[i], NULL) != 0)
        {
            printf("failed to join thread %d\n", i);
        }
        i++;
    }
	set_all_finished(philo);//////////////
	if(all_is_finished(philo))
	{
		
	printf("all is finished\n");
	// printf("here\n");
	// exit(0);
	}
	if (pthread_join(watcher, NULL) != 0)
    {
        printf("failed to join watcher thread \n");
    }
	if (pthread_join(noter, NULL) != 0)
    {
        printf("failed to join noter thread\n");
    }

    return (exiter(0, philo, vars));
}