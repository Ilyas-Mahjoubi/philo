/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:39:20 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/09 19:05:12 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	lock_first_fork(t_philo *philo, t_fork *first_fork)
{
	pthread_mutex_lock(&first_fork->fork);
	if (philo->table->end_simulation)
	{
		pthread_mutex_unlock(&first_fork->fork);
		return (0);
	}
	safe_print(philo, "has taken a fork");
	return (1);
}

static int	lock_second_fork(t_philo *philo, t_fork *first_fork,
				t_fork *second_fork)
{
	pthread_mutex_lock(&second_fork->fork);
	if (philo->table->end_simulation)
	{
		pthread_mutex_unlock(&second_fork->fork);
		pthread_mutex_unlock(&first_fork->fork);
		return (0);
	}
	safe_print(philo, "has taken a fork");
	return (1);
}

void	take_forks(t_philo *philo)
{
	t_fork	*first_fork;
	t_fork	*second_fork;

	if (philo->table->end_simulation)
		return ;
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (!lock_first_fork(philo, first_fork))
		return ;
	usleep(100);
	if (!lock_second_fork(philo, first_fork, second_fork))
		return ;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

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
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

static int	check_philo_death(t_table *table, int i)
{
	long	now;
	long	last_meal;
	long	time_diff;

	pthread_mutex_lock(&table->meal_mutex);
	now = get_time();
	last_meal = table->philos[i].last_meal_time;
	time_diff = now - last_meal;
	if (time_diff > table->time_to_die)
	{
		safe_print(&table->philos[i], "died");
		table->end_simulation = 1;
		pthread_mutex_unlock(&table->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->meal_mutex);
	return (0);
}

void	*monitor_deaths(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!table->end_simulation)
	{
		i = 0;
		while (i < table->philo_nbr)
		{
			if (check_philo_death(table, i))
				return (NULL);
			i++;
		}
		usleep(2000);
	}
	return (NULL);
}
