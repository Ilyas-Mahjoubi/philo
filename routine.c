/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:39:20 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/05 00:49:11 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_print(t_philo *philo, char *message)
{
	long	timestamp;
	if (philo->table->end_simulation)
		return ;
	timestamp = get_time() - philo->table->start_simulation;
	printf("%ld %d %s\n", timestamp, philo->id, message);
}

void	take_forks(t_philo *philo)
{
	// always take lower num fork first to avoid deaflock
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->fork);
		safe_print(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->fork);
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
    
    // update last_meal time
    philo->last_meal_time = get_time();
    safe_print(philo, "is eating");
    usleep(philo->table->time_to_eat);
    philo->eat_count++;
    //check is philo full
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
void	*philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	
	//set initial last meal time
	philo->last_meal_time = get_time();
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!philo->table->end_simulation && !philo->full)
	{
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int i;

	table->start_simulation = get_time();
	
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
	// wait for all the threads to finish
	i = 0;
	while (i < table->philo_nbr)
	{
		if (pthread_join(table->philos[i].thread_id, NULL) != 0)
		{
			eroor_exit("Error: Failed to join philosopher thread.\n");
		}
		i++;
	}
}

void	clean(t_table *table)
{
	int i;

	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i].fork);
		i++;
	}
	if (table->forks)
		free(table->forks);
	if(table->philos)
		free(table->philos);
}