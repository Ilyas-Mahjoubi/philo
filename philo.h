/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:47:06 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/06/07 14:29:52 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h> // mutex : init destroy lock unlock
                    // threads : create join detach
#include <sys/time.h> // GETTIMEOFDAY
#include <limits.h> // INT_MAX

typedef pthread_mutex_t t_mtx;
typedef struct s_table t_table;

typedef struct s_fork
{
    t_mtx   fork;
    int		fork_id;
}   	t_fork;

typedef struct s_philo
{
    int				id;
    long			eat_count;
    bool            full;
    long            last_meal_time; // time passed from last meal
    t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_t		thread_id; // a philo is a therad
	t_table			*table;
}	t_philo;


typedef struct s_table
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	nbr_limit_meals; // [5] or flag if -1
	long	start_simulation;
	bool	end_simulation;	// a philo dies or alll philo is full
	t_fork	*forks; // array for forks [0, 1, 2, 3, 4]
	t_philo	*philos; // array for philos
	t_mtx	print_mutex;
	t_mtx   meal_mutex; // protects last_meal_time updates
}			t_table;





void eroor_exit(const char *error);
void parse_input(t_table *table, char *av[]);
long	get_time(void);
void	safe_print(t_philo *philo, char *message);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
void philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philo_routine(void *arg);
void	dinner_start(t_table *table);
void	clean(t_table *table);
void init_table(t_table *table);
void *monitor_deaths(void *arg);
bool all_philosophers_full(t_table *table);
void *monitor_deaths(void *arg);

#endif