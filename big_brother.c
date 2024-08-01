#include "philosim.h"

void* sim_run_big_brother(void *arg) {
    t_simulation *sim;
    pthread_t handler;

    sim = (t_simulation*)arg;
    pthread_create(&handler, NULL, &big_brother_routine, (void*)sim);
    pthread_join(handler,NULL);

}

void* big_brother_routine(void *arg) {
        int tmp;
        t_philo *philo;
        t_simulation *sim;

        sim = (t_simulation*)arg;

        while(sim->running) {
            tmp = 0;
            while(tmp < sim->num_philos) {
                philo = &sim->philos[tmp];
                if(get_time_ms() - philo->last_meal > sim->time_to_die) {
                    sim->running = 0;
                    sim_trigger_event(DIED,philo);
                    return (NULL);
                }
                if(philo->sim->philos_full == sim->num_meals) {
                    sim->running = 0;
                    return (NULL);
                }
            }
            usleep(100);
        }
}