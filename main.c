#include "philo.h"

void    thinking(int id)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("seconds %ld , micors %ld", tv.tv_sec - get_vars()->initial_timeval.tv_sec, tv.tv_usec - get_vars()->initial_timeval.tv_usec);
    printf(" %d is thinking\n", id + 1);
}

void    eating(int id)
{
    struct timeval tv;

    pthread_mutex_lock(&get_vars()->forks[id]);
    if (id == get_vars()->number_of_philosophers - 1)
        pthread_mutex_lock(&get_vars()->forks[0]);
    else
        pthread_mutex_lock(&get_vars()->forks[id + 1]);
    gettimeofday(&tv, NULL);
    printf("seconds %ld , micors %ld", tv.tv_sec - get_vars()->initial_timeval.tv_sec, tv.tv_usec - get_vars()->initial_timeval.tv_usec);
    printf(" %d is eating\n", id + 1);
    usleep(get_vars()->time_to_eat);

     pthread_mutex_unlock(&get_vars()->forks[id]);
    if (id == get_vars()->number_of_philosophers - 1)
        pthread_mutex_unlock(&get_vars()->forks[0]);
    else
        pthread_mutex_unlock(&get_vars()->forks[id + 1]);
}
void    sleeping(int id)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("seconds %ld , micors %ld", tv.tv_sec - get_vars()->initial_timeval.tv_sec, tv.tv_usec - get_vars()->initial_timeval.tv_usec);
    printf(" %d is sleeping\n", id + 1);
    usleep(get_vars()->time_to_sleep);

}


t_vars *get_vars(void)
{
    static t_vars   vars;
    return  (&vars);
}

void    *routine(void *id)
{
    while (1)
    {
        thinking(*(int *)id);
        eating(*(int *)id);
        sleeping(*(int *)id);
    }
    // printf ("my id is %d\n", *(int *)id);
    return NULL;
}

void init_vars(char **av, int ac)
{
    t_vars *vars;
    int i;

    vars = get_vars();
    vars->number_of_philosophers = ft_atoi(av[1]);
    vars->time_to_die = ft_atoi(av[2]);
    vars->time_to_eat = ft_atoi(av[3]);
    vars->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        vars->umber_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    else
        vars->umber_of_times_each_philosopher_must_eat = -1;
    vars->philo = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers); 
    vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers); 
    gettimeofday(&(vars->initial_timeval), NULL);
    i = 0;
    while (i < vars->number_of_philosophers)
    {
        pthread_mutex_init(&(vars->forks[i]), NULL);
        i++;
    }
}

int main(int ac, char **av)
{
    int i;
    int *philo_id;
    int *joined;

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
        if (pthread_create(&(get_vars()->philo[i]), NULL, routine, philo_id) != 0)
        {
            printf("failed to create thread\n");
            exiter(1);
        }
        i++;
    }
    i = 0;
    while (i < get_vars()->number_of_philosophers)
    {
        if (pthread_join(get_vars()->philo[i], (void **)&joined) != 0)
        {
            printf("failed to join thread %d\n", i);
            free(joined);
        }
        i++;
    }
    exiter(0);

    return 0;
}
