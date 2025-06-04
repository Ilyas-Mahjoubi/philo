/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:52:58 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/04 23:28:51 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_forks(t_table *table)
{
	int i;

	table->forks = malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
		eroor_exit("Error: Memory allocation failed for forks.\n");
	i = 0;
	// initialize each fork
	while (i < table->philo_nbr)
	{
		if (pthread_mutex_init(&table->forks[i].fork, NULL) != 0)
			eroor_exit("Error: Failed to initialize fork mutex.\n");
		table->forks[i].fork_id = i;
		i++;
	}
}

static void	init_philos(t_table *table)
{
	int i;

	table->philos = malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
		roor_exit("Error: Memory allocation failed for philosophers.\n");
	i = 0;
	// initialize each philo
	while (i < table->philo_nbr)
	{
		table->philos[i].id = i + 1; // philos are numb
		table->philos[i].eat_count = 0;
		table->philos[i].full = false;
		table->philos[i].last_meal_time = 0;
		table->philos->table = table; // pointer back to table
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->philo_nbr];
		i++;
	}
}

void init_table(t_table *table)
{
	table->start_simulation = 0;
	table->end_simulation = false;
	init_forks(table);
	init_philos(table);
}