#include "philo.h"

size_t  get_time_milliseconds(void)
{
    struct timeval tv;
    size_t  time;

    gettimeofday(&tv, NULL);
    time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return (time);
}

void    logger(int id, char *msg)
{
    pthread_mutex_lock(&get_vars()->log_lock);
    if (get_vars()->simulation_finished == 1)
    {
         pthread_mutex_unlock(&get_vars()->log_lock);
         return ;
    }
    printf("%lu %d %s\n", get_time_milliseconds() - get_vars()->initial_timeval, id + 1, msg);
    pthread_mutex_unlock(&get_vars()->log_lock);
}

void    thinking(t_philo *philo)
{
    logger(philo->id, "is thinking");
}

void eating(t_philo *philo)
{
    if (philo->id == get_vars()->number_of_philosophers - 1) {
        pthread_mutex_lock(&get_vars()->forks[0]);
        logger(philo->id, "has taken a fork");
        pthread_mutex_lock(&get_vars()->forks[philo->id]);
        logger(philo->id, "has taken a fork");
    } else {
        pthread_mutex_lock(&get_vars()->forks[philo->id]);
        logger(philo->id, "has taken a fork");
        pthread_mutex_lock(&get_vars()->forks[philo->id + 1]);
        logger(philo->id, "has taken a fork");
    }
    philo->last_meal = get_time_milliseconds();
    logger(philo->id, "is eating");
    usleep(get_vars()->time_to_eat * 1000);
    pthread_mutex_unlock(&get_vars()->forks[philo->id]);
    if (philo->id == get_vars()->number_of_philosophers - 1)
        pthread_mutex_unlock(&get_vars()->forks[0]);
    else
        pthread_mutex_unlock(&get_vars()->forks[philo->id + 1]);
}
void    sleeping(t_philo *philo)
{
    logger(philo->id, "is sleeping");
    usleep(get_vars()->time_to_sleep * 1000);
}


t_vars *get_vars(void)
{
    static t_vars   vars;
    return  (&vars);
}

int died(t_philo *philo)
{
    if (get_time_milliseconds() - philo->last_meal >= get_vars()->time_to_die)
    {
        pthread_mutex_lock(&(philo->watcher_lock));
        get_vars()->simulation_finished = 1;
        printf("%lu %d died\n", get_time_milliseconds() - get_vars()->initial_timeval, philo->id + 1);
        // exiter(0);///
        pthread_mutex_unlock(&(philo->watcher_lock));
        return (1);
    }
    return (0);
}

void    *watching(void *philo)
{
    while (get_vars()->simulation_finished != 1 && !died(philo))
    {
        usleep(1234);
    }
    pthread_mutex_destroy(&(((t_philo *)philo)->watcher_lock));
    return philo;
}

void    *routine(void *philo)
{
    if (pthread_create(&(get_vars()->watchers[((t_philo *)philo)->id]), NULL, watching, philo) != 0)
    {
        printf("failed to create thread\n");
        exiter(1);
    }
    while (get_vars()->simulation_finished != 1)
    {
        thinking((t_philo *)philo);
        eating((t_philo *)philo);
        sleeping((t_philo *)philo);
    }
    if (pthread_join(get_vars()->watchers[*(int *)philo], NULL) != 0)
    {
        printf("failed to join thread\n");
    }
    return philo;
}

t_philo **init_vars(char **av, int ac)
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
    vars->philos = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers);
    vars->watchers = (pthread_t *)malloc(sizeof(pthread_t) * vars->number_of_philosophers);
    vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers); 
    i = 0;
    t_philo **philo = (t_philo **)malloc(sizeof(t_philo *) * vars->number_of_philosophers);
    vars->initial_timeval = get_time_milliseconds();
    while (i < vars->number_of_philosophers)
    {
        philo[i] = (t_philo *)malloc(sizeof(t_philo) * 1);
        philo[i]->id = i;
        pthread_mutex_init(&(philo[i]->watcher_lock), NULL);
        philo[i]->last_meal = get_time_milliseconds();//get_vars()->initial_timeval;
        pthread_mutex_init(&(vars->forks[i]), NULL);
        i++;
    }
    vars->simulation_finished = 0;
    return (philo);
}

int main(int ac, char **av)
{
    int i;
    t_philo **philo;
    t_philo *joined;

    if (ac != 5 && ac != 6)
    {
        printf("wrong args\n");
        return (1);
    }
    philo = init_vars(av, ac);
    i = 0;
    while (i < get_vars()->number_of_philosophers)
    {
        if (pthread_create(&(get_vars()->philos[i]), NULL, routine, philo[i]) != 0)
        {
            printf("failed to create thread\n");
            exiter(1);
        }
        i++;
    }
    i = 0;
    while (i < get_vars()->number_of_philosophers)
    {
        if (pthread_join(get_vars()->philos[i], (void **)&joined) != 0)
        {
            printf("failed to join thread %d\n", i);
            continue ;
        }
            pthread_mutex_destroy(&(joined->watcher_lock));
            free(joined);
        i++;
    }
    exiter(0);

    return 0;
}