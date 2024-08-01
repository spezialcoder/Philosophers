#include "philosim.h"
#include "routines.h"

uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000+tv.tv_usec/1000);
}

void sim_delay(uint32_t time, t_simulation* sim) {
    register uint64_t wakeup;

    wakeup = get_time_ms()+time;
    while(get_time_ms() < wakeup) {
        if(!sim->running)
            return;
        usleep(50);
    }
}

void sim_trigger_event(enum EVENT type, t_philo *p) {
    uint64_t timestamp;

    pthread_mutex_lock(&p->sim->write_lock);
    timestamp = get_time_ms()-(p->sim->start_time_stamp);
    if(type == FORK_TAKEN &&
        p->sim->running) {
        printf("%llu %d has taken a fork\n",timestamp,p->philo_id);
    } else if(type == EATING &&
        p->sim->running) {
        printf("%llu %d is eating\n",timestamp,p->philo_id);
    } else if(type == SLEEPING &&
        p->sim->running) {
        printf("%llu %d is sleeping\n",timestamp,p->philo_id);
    } else if(type == THINKING &&
        p->sim->running) {
        printf("%llu %d is thinking\n",timestamp,p->philo_id);
    } else if(type == DIED) {
        printf("%llu %d died\n",timestamp,p->philo_id);
    }

    pthread_mutex_unlock(&p->sim->write_lock);
}

static void wait_till_termination(t_simulation *sim) {
    int tmp;

    tmp = 0;
    while(tmp < sim->num_philos) {
        pthread_join(sim->threads[tmp],NULL);
        tmp++;
    }
}

void sim_run(int num_philos, int time_to_eat, int time_to_sleep,
             int time_to_die, int num_meals) {
    t_simulation sim;
    int tmp;
    sim = (t_simulation) {num_philos, time_to_eat, time_to_sleep,
                          time_to_die, num_meals, 0,0, 0, 0, 0, 1, 0, 0};
    sim.running = 1;
    sim.philos = (t_philo *) malloc(num_philos * sizeof(t_philo));
    sim.threads = malloc(num_philos * sizeof(pthread_t));
    sim.forks = malloc(num_philos * sizeof(pthread_mutex_t));
    pthread_mutex_init(&sim.write_lock, NULL);
    pthread_mutex_init(&sim.pf_lock,NULL);
    tmp = 0;
    while (tmp < num_philos)
        pthread_mutex_init(&sim.forks[tmp++], NULL);
    tmp = 0;
    sim.start_time_stamp = get_time_ms();
    while (tmp < num_philos) {
        sim.philos[tmp] = (t_philo) {tmp + 1, 0, &sim,0};
        pthread_create(&sim.threads[tmp], NULL, &sim_philo_routine, (void *) &sim.philos[tmp]);
        tmp++;
    }
    tmp = 0;
    sim_run_big_brother((void*)&sim);
    wait_till_termination(&sim);
    while (tmp < num_philos) {
        pthread_mutex_destroy(&sim.forks[tmp++]);
    }
    pthread_mutex_destroy(&sim.write_lock);
    pthread_mutex_destroy(&sim.pf_lock);
    free(sim.forks);
    free(sim.threads);
    free(sim.philos);
}