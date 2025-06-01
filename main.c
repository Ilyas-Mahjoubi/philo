/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:01:26 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/01 20:12:41 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char *av[])
{
	t_table	table;
	if (ac == 5 || ac == 6) // correct input
	{
		parse_input(&table, av); // parse input arguments
		init_table(&table); // initialize table and philosophers
		dinner_start(&table); // start the dinner simulation
		clean(&table); // clean up resources
	}
	else
	{
		eroor_exit("Error: Wrong number of arguments.\n");
	}
}