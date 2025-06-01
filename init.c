/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:52:58 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/01 20:54:18 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_table(t_table *table)
{
	table->start_simulation = 0;
	table->end_simulation = false;
	table->forks = malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
		eroor_exit("Error: Memory allocation failed for forks.\n");
	
}