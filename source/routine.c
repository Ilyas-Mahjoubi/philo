/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:39:20 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/05 20:13:37 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void safe_print(t_philo *philo, char *message)
{
    pthread_mutex_lock(&philo->table->print_mutex);
    if (!philo->table->end_simulation)
    {
        long timestamp = get_time() - philo->table->start_simulation;
        printf("%ld %d %s\n", timestamp, philo->id, message);
    }
    pthread_mutex_unlock(&philo->table->print_mutex);
}

void take_forks(t_philo *philo)
{
    if (philo->table->end_simulation) return;
    
    // always take lower num fork first to avoid deadlock
    if (philo->left_fork->fork_id < philo->right_fork->fork_id)
    {
        pthread_mutex_lock(&philo->left_fork->fork);
        if (philo->table->end_simulation) {
            pthread_mutex_unlock(&philo->left_fork->fork);
            return;
        }
        safe_print(philo, "has taken a fork");
        
        pthread_mutex_lock(&philo->right_fork->fork);
        if (philo->table->end_simulation) {
            pthread_mutex_unlock(&philo->right_fork->fork);
            pthread_mutex_unlock(&philo->left_fork->fork);
            return;
        }
        safe_print(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->right_fork->fork);
        if (philo->table->end_simulation) {
            pthread_mutex_unlock(&philo->right_fork->fork);
            return;
        }
        safe_print(philo, "has taken a fork");
        
        pthread_mutex_lock(&philo->left_fork->fork);
        if (philo->table->end_simulation) {
            pthread_mutex_unlock(&philo->left_fork->fork);
            pthread_mutex_unlock(&philo->right_fork->fork);
            return;
        }
        safe_print(philo, "has taken a fork");
    }
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_unlock(&philo->right_fork->fork);
}

void philo_eat(t_philo *philo)
{
    if (philo->table->end_simulation) return;
    
    take_forks(philo);
    
    if (philo->table->end_simulation) {
        drop_forks(philo);
        return;
    }

    // Update last_meal time with DEDICATED mutex protection
    pthread_mutex_lock(&philo->table->meal_mutex);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->table->meal_mutex);

    safe_print(philo, "is eating");
    
    if (philo->table->end_simulation) {
        drop_forks(philo);
        return;
    }
    
    usleep(philo->table->time_to_eat);
    philo->eat_count++;

    //check if philo is full
    if (philo->table->nbr_limit_meals != -1 &&
        philo->eat_count >= philo->table->nbr_limit_meals)
    {
        philo->full = true;
    }
    drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	safe_print(philo, "is sleeping");
	usleep(philo->table->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	safe_print(philo, "is thinking");
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
    // Stagger even-numbered philosophers to prevent deadlock
    if (philo->id % 2 == 0)
        usleep(1000);
    
    while (!philo->table->end_simulation && !philo->full)
    {
        // Check before each action
        if (philo->table->end_simulation) break;
        philo_think(philo);
        
        if (philo->table->end_simulation) break;
        philo_eat(philo);
        
        if (philo->table->end_simulation) break;
        philo_sleep(philo);
    }
    return (NULL);
}

void dinner_start(t_table *table)
{
    pthread_t monitor_thread;
    int i;

    table->start_simulation = get_time();
    
    // Set initial last_meal_time for all philosophers WITH MUTEX PROTECTION
    pthread_mutex_lock(&table->meal_mutex);  // ADD THIS
    i = 0;
    while (i < table->philo_nbr)
    {
        table->philos[i].last_meal_time = table->start_simulation;
        i++;
    }
    pthread_mutex_unlock(&table->meal_mutex);  // ADD THIS
    
    // Rest of the function remains the same...
    // Create philosopher threads
    i = 0;
    while (i < table->philo_nbr)
    {
        if (pthread_create(&table->philos[i].thread_id, NULL,
            philo_routine, &table->philos[i]) != 0)
        {
            eroor_exit("Error: Failed to create philosopher thread.\n");
        }
        i++;
    }
    
    // Create monitor thread
    if (pthread_create(&monitor_thread, NULL, monitor_deaths, table) != 0)
        eroor_exit("Error: Failed to create monitor thread.\n");
    
    // Wait for monitor thread (it will end when simulation ends)
    pthread_join(monitor_thread, NULL);
    
    // Wait for all philosopher threads to finish
    i = 0;
    while (i < table->philo_nbr)
    {
		// if (table->philo_nbr == 1)
		// 	return ;
        pthread_join(table->philos[i].thread_id, NULL);
        i++;
    }
}

void clean(t_table *table)
{
    int i = 0;
    while (i < table->philo_nbr)
    {
        pthread_mutex_destroy(&table->forks[i].fork);
        i++;
    }
    pthread_mutex_destroy(&table->print_mutex);
    pthread_mutex_destroy(&table->meal_mutex);  // ADD THIS
    if (table->forks)
        free(table->forks);
    if (table->philos)
        free(table->philos);
}

void *monitor_deaths(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    long current_time;
    long last_meal;
    
    while (!table->end_simulation)
    {
        i = 0;
        current_time = get_time();
        
        while (i < table->philo_nbr && !table->end_simulation)
        {
            // READ last_meal_time with mutex protection
            pthread_mutex_lock(&table->meal_mutex);
            last_meal = table->philos[i].last_meal_time;
            pthread_mutex_unlock(&table->meal_mutex);
            
            if (current_time - last_meal > table->time_to_die)
            {
                pthread_mutex_lock(&table->print_mutex);
                if (!table->end_simulation)
                {
                    long timestamp = current_time - table->start_simulation;
                    printf("%ld %d died\n", timestamp, table->philos[i].id);
                    table->end_simulation = true;
                }
                pthread_mutex_unlock(&table->print_mutex);
                return (NULL);
            }
            i++;
        }
        
        // Check if all philosophers are full
        if (all_philosophers_full(table))
        {
            table->end_simulation = true;
            return (NULL);
        }
        usleep(1000); // Check every millisecond
    }
    return (NULL);
}

bool all_philosophers_full(t_table *table)
{
    int i = 0;
    if (table->nbr_limit_meals == -1)
        return false;
    
    while (i < table->philo_nbr)
    {
        if (!table->philos[i].full)
            return false;
        i++;
    }
    return true;
}