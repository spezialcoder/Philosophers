/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:32:25 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:57:13 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosim.h"
#include "routines.h"

t_uint64_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	sim_delay(t_uint32_t time, t_simulation *sim)
{
	register t_uint64_t	wakeup;

	wakeup = get_time_ms() + time;
	while (get_time_ms() < wakeup)
	{
		if (!sim->running)
			return ;
		usleep(100);
	}
}

void	sim_trigger_event(enum e_EVENT type, t_philo *p)
{
	t_uint64_t	timestamp;

	pthread_mutex_lock(&p->sim->write_lock);
	timestamp = get_time_ms() - (p->sim->start_time_stamp);
	if (type == FORK_TAKEN && p->sim->running)
	{
		printf("%llu %d has taken a fork\n", timestamp, p->philo_id);
	}
	else if (type == EATING && p->sim->running)
	{
		printf("%llu %d is eating\n", timestamp, p->philo_id);
	}
	else if (type == SLEEPING && p->sim->running)
	{
		printf("%llu %d is sleeping\n", timestamp, p->philo_id);
	}
	else if (type == THINKING && p->sim->running)
	{
		printf("%llu %d is thinking\n", timestamp, p->philo_id);
	}
	else if (type == DIED)
	{
		printf("%llu %d died\n", timestamp, p->philo_id);
	}
	pthread_mutex_unlock(&p->sim->write_lock);
}

static void	wait_till_termination(t_simulation *sim)
{
	t_uint32_t	tmp;

	tmp = 0;
	while (tmp < sim->num_philos)
	{
		pthread_join(sim->threads[tmp], NULL);
		tmp++;
	}
}

void	sim_run(t_parameters param)
{
	t_simulation	sim;
	int				tmp;

	init_sim(&sim, param);
	tmp = 0;
	while (tmp < param.num_philos)
		pthread_mutex_init(&sim.forks[tmp++], NULL);
	tmp = -1;
	sim.start_time_stamp = get_time_ms();
	while (++tmp < param.num_philos)
	{
		sim.philos[tmp] = (t_philo){tmp + 1, 0, &sim, 0};
		pthread_create(&sim.threads[tmp], NULL, &sim_philo_routine,
			(void *)&sim.philos[tmp]);
	}
	tmp = 0;
	sim_run_big_brother((void *)&sim);
	wait_till_termination(&sim);
	while (tmp < param.num_philos)
		pthread_mutex_destroy(&sim.forks[tmp++]);
	pthread_mutex_destroy(&sim.write_lock);
	pthread_mutex_destroy(&sim.pf_lock);
	free(sim.forks);
	free(sim.threads);
	free(sim.philos);
}
