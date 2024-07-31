#include "routines.h"

static void eat_routine(t_philo *p);
static void think_routine();

void* sim_philo_routine(void *arg) {
    t_philo *p;

    p = (t_philo*)arg;
    if(p->philo_id%2 == 0) {
        sim_delay(200);
    }
    while(1) {
        eat_routine(p);
    }
    return (NULL);
}

static inline void accquire_forks(t_philo *p) {
    pthread_mutex_lock(&p->sim->forks[(
            p->philo_id-1
            ) % p->sim->num_philos]);
    sim_trigger_event(FORK_TAKEN,p);
    pthread_mutex_lock(&p->sim->forks[(
            p->philo_id
            )%p->sim->num_philos]);
    sim_trigger_event(FORK_TAKEN,p);
}

static inline void release_forks(t_philo *p) {
    pthread_mutex_unlock(&p->sim->forks[(
                                              p->philo_id-1
                                      ) % p->sim->num_philos]);
    pthread_mutex_unlock(&p->sim->forks[(
                                              p->philo_id
                                      )%p->sim->num_philos]);
}

static void eat_routine(t_philo *p) {
    accquire_forks(p);
    sim_trigger_event(EATING,p);
    sim_delay(p->sim->time_to_eat);
    release_forks(p);
    sim_trigger_event(SLEEPING,p);
    sim_delay(p->sim->time_to_sleep);
}

static void think_routine(t_philo *p) {
    sim_trigger_event(THINKING,p);
}