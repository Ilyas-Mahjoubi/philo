/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utlis.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:06:07 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/09 19:07:44 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	eroor_exit(const char *error)
{
	printf("%s", error);
	exit(1);
}

void	clean(t_table *table)
{
	int	i;

	i = 0;
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

void	safe_print(t_philo *philo, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (!philo->table->end_simulation)
	{
		timestamp = get_time() - philo->table->start_simulation;
		printf("%ld %d %s\n", timestamp, philo->id, message);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_unlock(&philo->right_fork->fork);
}

bool	all_philosophers_full(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (!table->philos[i].full)
			return (false);
		i++;
	}
	return (true);
}
