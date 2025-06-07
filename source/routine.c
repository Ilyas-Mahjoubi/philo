/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:39:20 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/07 19:04:42 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
	if (philo->table->end_simulation) 
		return;
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		if (philo->table->end_simulation) {
			pthread_mutex_unlock(&philo->left_fork->fork);
			return;
		}
		safe_print(philo, "has taken a fork");
		usleep(100);
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
	take_forks(philo);
	safe_print(philo, "is eating");

	pthread_mutex_lock(&philo->table->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->table->meal_mutex);
	usleep(philo->table->time_to_eat);
	philo->eat_count++;
	if (philo->table->nbr_limit_meals != -1 && philo->eat_count >= philo->table->nbr_limit_meals)
		philo->full = true;
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
	t_philo *philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->table->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->table->meal_mutex);
	if (philo->id % 2 == 0)
		usleep(15000);
	while (!philo->table->end_simulation)
	{
		philo_eat(philo);
		if (philo->table->end_simulation || philo->full)
			break;
		philo_sleep(philo);
		philo_think(philo);
	}
	return NULL;
}

void dinner_start(t_table *table)
{
    pthread_t monitor_thread;
    int i;

    table->start_simulation = get_time();
    pthread_mutex_lock(&table->meal_mutex);
    i = 0;
    while (i < table->philo_nbr)
    {
        table->philos[i].last_meal_time = table->start_simulation;
        i++;
    }
    pthread_mutex_unlock(&table->meal_mutex);
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
    if (pthread_create(&monitor_thread, NULL, monitor_deaths, table) != 0)
        eroor_exit("Error: Failed to create monitor thread.\n");
    pthread_join(monitor_thread, NULL);
    i = 0;
    while (i < table->philo_nbr)
    {
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
    pthread_mutex_destroy(&table->meal_mutex);
    if (table->forks)
        free(table->forks);
    if (table->philos)
        free(table->philos);
}

#include "philo.h"

bool all_philosophers_full(t_table *table)
{
	int i;
	for (i = 0; i < table->philo_nbr; i++)
	{
		if (!table->philos[i].full)
			return false;
	}
	return true;
}

/* void *monitor_deaths(void *arg)
{
	t_table *table = (t_table *)arg;

	while (!table->end_simulation)
	{
		for (int i = 0; i < table->philo_nbr; i++)
		{
			pthread_mutex_lock(&table->meal_mutex);
			long now = get_time();
			long last_meal = table->philos[i].last_meal_time;

			if ((now - last_meal) > table->time_to_die)
			{
				safe_print(&table->philos[i], "died");
				table->end_simulation = true;
				pthread_mutex_unlock(&table->meal_mutex);
				return NULL;
			}
			pthread_mutex_unlock(&table->meal_mutex);
		}
		if (table->nbr_limit_meals != -1 && all_philosophers_full(table))
		{
			table->end_simulation = true;
			return NULL;
		}
		usleep(2000);
	}
	return NULL;
} */


void *monitor_deaths(void *arg)
{
    t_table *table = (t_table *)arg;

    while (!table->end_simulation)
    {
        for (int i = 0; i < table->philo_nbr; i++)
        {
            pthread_mutex_lock(&table->meal_mutex);
            long now = get_time();
            long last_meal = table->philos[i].last_meal_time;
            long time_diff = now - last_meal;

            // ADD THIS DEBUG LINE
            if (i < 3) // Only first 3 philosophers
                printf("DEBUG: Philo %d - time since last meal: %ld ms\n", i+1, time_diff);

            if (time_diff > table->time_to_die)
            {
                safe_print(&table->philos[i], "died");
                table->end_simulation = true;
                pthread_mutex_unlock(&table->meal_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&table->meal_mutex);
        }
        usleep(2000);
    }
    return NULL;
}
