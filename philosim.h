/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosim.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:28:58 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:56:45 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef unsigned char		t_uint8_t;
typedef unsigned short		t_uint16_t;
typedef unsigned int		t_uint32_t;
typedef unsigned long long	t_uint64_t;

enum						e_EVENT
{
	FORK_TAKEN = 0,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_simulation	t_simulation;
typedef struct s_philo		t_philo;

typedef struct s_parameters
{
	int						num_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						num_meals;
}							t_parameters;

struct						s_simulation
{
	t_uint32_t				num_philos;
	t_uint32_t				time_to_eat;
	t_uint32_t				time_to_sleep;
	t_uint32_t				time_to_die;
	t_uint32_t				num_meals;
	t_uint64_t				start_time_stamp;
	pthread_t				*threads;
	pthread_mutex_t			*forks;
	pthread_mutex_t			write_lock;
	t_philo					*philos;
	volatile t_uint8_t		running;
	pthread_mutex_t			pf_lock;
	t_uint32_t				philos_full;
};

struct						s_philo
{
	int						philo_id;
	t_uint64_t				last_meal;
	t_simulation			*sim;
	t_uint32_t				meal_count;
};

t_uint64_t					get_time_ms(void);
void						sim_delay(t_uint32_t time, t_simulation *sim);
void						sim_trigger_event(enum e_EVENT type, t_philo *p);
void						sim_run(t_parameters param);
void						*big_brother_routine(void *arg);
void						*sim_run_big_brother(void *arg);
void						init_sim(t_simulation *sim, t_parameters param);