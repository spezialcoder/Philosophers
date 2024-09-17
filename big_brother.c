/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_brother.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:30:54 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:55:28 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosim.h"

void	init_sim(t_simulation *sim, t_parameters param)
{
	*sim = (t_simulation){param.num_philos, param.time_to_eat,
		param.time_to_sleep, param.time_to_die, param.num_meals, 0,
		.running = 1, .philos_full = 0};
	sim->philos = (t_philo *)malloc(param.num_philos * sizeof(t_philo));
	sim->threads = malloc(param.num_philos * sizeof(pthread_t));
	sim->forks = malloc(param.num_philos * sizeof(pthread_mutex_t));
	pthread_mutex_init(&sim->write_lock, NULL);
	pthread_mutex_init(&sim->pf_lock, NULL);
}

void	*sim_run_big_brother(void *arg)
{
	t_simulation	*sim;
	pthread_t		handler;

	sim = (t_simulation *)arg;
	pthread_create(&handler, NULL, &big_brother_routine, (void *)sim);
	pthread_join(handler, NULL);
	return (NULL);
}

void	*big_brother_routine(void *arg)
{
	t_uint32_t		tmp;
	t_uint64_t		lifetime;
	t_philo			*philo;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (sim->running)
	{
		tmp = 0;
		while (tmp < sim->num_philos)
		{
			philo = &sim->philos[tmp];
			if (philo->last_meal == 0)
				lifetime = sim->start_time_stamp;
			else
				lifetime = philo->last_meal;
			if (get_time_ms() - lifetime > sim->time_to_die)
				return (sim->running = 0, sim_trigger_event(DIED, philo), NULL);
			tmp++;
		}
		if (sim->philos_full == sim->num_philos && sim->num_philos)
			return (sim->running = 0, NULL);
		usleep(100);
	}
	return (NULL);
}
