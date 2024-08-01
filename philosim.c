#include "philosim.h"
#include "routines.h"

uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000+tv.tv_usec/1000);
}

void sim_delay(uint32_t time) {
    register uint64_t wakeup;

    wakeup = get_time_ms()+time;
    while(get_time_ms() < wakeup) {
        usleep(100);
    }
}

void sim_trigger_event(enum EVENT type, t_philo *p) {
    uint64_t timestamp;

    pthread_mutex_lock(&p->sim->write_lock);
    timestamp = get_time_ms()-(p->sim->start_time_stamp);
    if(type == FORK_TAKEN) {
        printf("%llu %d has taken a fork\n",timestamp,p->philo_id);
    } else if(type == EATING) {
        printf("%llu %d is eating\n",timestamp,p->philo_id);
    } else if(type == SLEEPING) {
        printf("%llu %d is sleeping\n",timestamp,p->philo_id);
    } else if(type == THINKING) {
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
    t_philo *p;
    int tmp;

    sim = (t_simulation){num_philos,time_to_eat,time_to_sleep,
                         time_to_die, num_meals,0,0,0,0};
    p = (t_philo*)malloc(num_philos*sizeof(t_philo));
    sim.threads = malloc(num_philos*sizeof(pthread_t));
    sim.forks = malloc(num_philos*sizeof(pthread_mutex_t));
    sim.start_time_stamp = get_time_ms();
    tmp = 0;
    while(tmp < num_philos)
        pthread_mutex_init(&sim.forks[tmp++],NULL);
    tmp = 0;
    while(tmp < num_philos) {
        p[tmp] = (t_philo){tmp+1,0,&sim};
        pthread_create(&sim.threads[tmp], NULL, &sim_philo_routine, (void*)&p[tmp]);
        tmp++;
    }
    tmp = 0;
    wait_till_termination(&sim);
    while(tmp < num_philos) {
        pthread_mutex_destroy(&sim.forks[tmp++]);
    }
    free(sim.forks);
    free(sim.threads);
    free(p);
}