#include "philosim.h"

void* sim_run_big_brother(void *arg) {
    t_simulation *sim;
    pthread_t handler;

    sim = (t_simulation*)arg;
    pthread_create(&handler, NULL, &big_brother_routine, (void*)sim);
    pthread_join(handler,NULL);
	return (NULL);
}

void* big_brother_routine(void *arg) {
        uint32_t tmp;
        uint64_t lifetime;
        t_philo *philo;
        t_simulation *sim;

        sim = (t_simulation*)arg;

        while(sim->running) {
            tmp = 0;
            while(tmp < sim->num_philos) {
                philo = &sim->philos[tmp];
                if(philo->last_meal == 0) {
                    lifetime = sim->start_time_stamp;
                } else {
                    lifetime = philo->last_meal;
                }
                if(get_time_ms() - lifetime > sim->time_to_die) {
                    sim->running = 0;
                    sim_trigger_event(DIED,philo);
                    return (NULL);
                }
                tmp++;
            }
            if(sim->philos_full == sim->num_philos && sim->num_philos) {

                sim->running = 0;
                return (NULL);
            }
            usleep(100);
        }
		return (NULL);
}
