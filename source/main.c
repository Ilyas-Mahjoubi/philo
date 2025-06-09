/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:01:26 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/09 18:53:31 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int ac, char *av[])
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		parse_input(&table, av);
		init_table(&table);
		dinner_start(&table);
		clean(&table);
	}
	else
	{
		eroor_exit("Error: Wrong number of arguments.\n");
	}
	return (0);
}
