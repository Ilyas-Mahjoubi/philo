/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utilis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 18:23:51 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/09 19:03:54 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	safe_print(philo, "is eating");
	pthread_mutex_lock(&philo->table->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->table->meal_mutex);
	usleep(philo->table->time_to_eat);
	philo->eat_count++;
	if (philo->table->nbr_limit_meals != -1
		&& philo->eat_count >= philo->table->nbr_limit_meals)
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
