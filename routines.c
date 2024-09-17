/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:19:55 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:47:02 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "routines.h"

static void			eat_routine(t_philo *p);
static void			think_routine(t_philo *p);

void	*sim_philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->sim->num_philos == 1)
	{
		while (p->sim->running)
			;
		return (NULL);
	}
	if (p->philo_id % 2 == 0)
	{
		think_routine(p);
	}
	while (p->sim->running)
	{
		eat_routine(p);
		think_routine(p);
	}
	return (NULL);
}

static inline void	accquire_forks(t_philo *p)
{
	pthread_mutex_lock(&p->sim->forks[(p->philo_id - 1) % p->sim->num_philos]);
	sim_trigger_event(FORK_TAKEN, p);
	pthread_mutex_lock(&p->sim->forks[(p->philo_id) % p->sim->num_philos]);
	sim_trigger_event(FORK_TAKEN, p);
}

static inline void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->sim->forks[(p->philo_id - 1)
		% p->sim->num_philos]);
	pthread_mutex_unlock(&p->sim->forks[(p->philo_id) % p->sim->num_philos]);
}

static void	eat_routine(t_philo *p)
{
	accquire_forks(p);
	sim_trigger_event(EATING, p);
	p->last_meal = get_time_ms();
	sim_delay(p->sim->time_to_eat, p->sim);
	p->meal_count++;
	if (p->meal_count == p->sim->num_meals && p->sim->num_meals)
	{
		pthread_mutex_lock(&p->sim->pf_lock);
		p->sim->philos_full++;
		pthread_mutex_unlock(&p->sim->pf_lock);
	}
	release_forks(p);
	sim_trigger_event(SLEEPING, p);
	sim_delay(p->sim->time_to_sleep, p->sim);
}

static void	think_routine(t_philo *p)
{
	long long	wakeup;

	wakeup = (long long)(p->sim->time_to_die - (get_time_ms() - p->last_meal));
	if (p->last_meal == 0)
	{
		wakeup = 100;
	}
	else if (wakeup < 0)
	{
		wakeup = 0;
	}
	else
	{
		wakeup = wakeup / 2;
	}
	if (p->last_meal)
		sim_trigger_event(THINKING, p);
	sim_delay(wakeup, p->sim);
}
