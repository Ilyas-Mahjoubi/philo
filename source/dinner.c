/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 18:38:34 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/09 18:50:54 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	init_meal_times(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->meal_mutex);
	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i].last_meal_time = table->start_simulation;
		i++;
	}
	pthread_mutex_unlock(&table->meal_mutex);
}

static void	create_philo_threads(t_table *table)
{
	int	i;

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
}

static void	join_all_threads(t_table *table, pthread_t monitor_thread)
{
	int	i;

	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}

void	dinner_start(t_table *table)
{
	pthread_t	monitor_thread;

	table->start_simulation = get_time();
	init_meal_times(table);
	create_philo_threads(table);
	if (pthread_create(&monitor_thread, NULL, monitor_deaths, table) != 0)
		eroor_exit("Error: Failed to create monitor thread.\n");
	join_all_threads(table, monitor_thread);
}
