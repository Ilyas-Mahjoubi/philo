/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:13:22 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/07 18:54:00 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"


static const char *validate_input(const char *str)
{
	int 		len;
	const char *number;
	
	len = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		eroor_exit("Error: Negative numbers are not allowed.\n");
	if (!(*str >= '0' && *str <= '9'))
		eroor_exit("Error: Invalid character in input.\n");
	number = str;
	while (*str >= '0' && *str <= '9')
	{
		len++;
		str++;
	}
	if (len > 10 || len == 0)
		eroor_exit("Error: Invalid number length.\n");
	return number;
}

static long ft_atol(const char *str)
{
	long num;

	num = 0;
	str = validate_input(str);
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	if (num < 0 || num > LONG_MAX)
	{
		eroor_exit("Error: Number out of range.\n");
	}
	return num;
}

void parse_input(t_table *table, char *av[])
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]); // convert to milliseconds
	table->time_to_eat = ft_atol(av[3]) * 1000; // convert to milliseconds
	table->time_to_sleep = ft_atol(av[4]) * 1000; // convert to milliseconds
	if(table->time_to_die < 60
		|| table->time_to_eat < 60000
		|| table->time_to_sleep < 60000)
		eroor_exit("Error: Time values must be at least 60 seconds.\n");
	if (av[5])
		table->nbr_limit_meals = ft_atol(av[5]);
	else
		table->nbr_limit_meals = -1; // no limit

	if (table->philo_nbr <= 0 || table->time_to_die <= 0 ||
		table->time_to_eat <= 0 || table->time_to_sleep <= 0 ||
		(av[5] && table->nbr_limit_meals <= 0))
	{
		eroor_exit("Error: Invalid input values.\n");
	}
}
