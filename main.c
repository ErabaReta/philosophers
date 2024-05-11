#include "philo.h"

t_vars *get_vars(void)
{
    static t_vars   vars;
    return  (&vars);
}

void    *routine(void *id)
{
    printf ("my id is %d\n", *(int *)id);
    return NULL;
}

void init_vars(char **av, int ac)
{
    t_vars *vars;

    vars = get_vars();
    vars->number_of_philosophers = ft_atoi(av[1]);
    vars->time_to_die = ft_atoi(av[2]);
    vars->time_to_eat = ft_atoi(av[3]);
    vars->time_to_sleep = ft_atoi(av[4]);
    if (ac == 5)
        vars->umber_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    else
        vars->umber_of_times_each_philosopher_must_eat = -1;
    vars->philo = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers); 
    vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers); 
    gettimeofday(&(vars->initial_timeval), NULL);
}

int main(int ac, char **av)
{
    pthread_t   pthread;
    int i;
    int *philo_id;

    if (ac != 5 && ac != 6)
    {
        printf("wrong args\n");
        return (1);
    }
    init_vars(av, ac);
    i = 0;
    while (i < get_vars()->number_of_philosophers)
    {
        philo_id = (int *)malloc(sizeof(int) * 1);
        *philo_id = i; 
        if (pthread_create(&pthread, NULL, routine, philo_id) != 0)
        {
            printf("failed to create thread\n");
            return 1;
        }
        i++;
    }
    i = 0;
    while (i < get_vars()->number_of_philosophers)
    {
        if (pthread_join(pthread, NULL) != 0)
       {
            printf("failed to join thread\n");
            return 1;
        }
        i++;
    }
    return 0;
}
