#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

enum EVENT {
    FORK_TAKEN=0,
    EATING,
    SLEEPING,
    THINKING,
    DIED
};

typedef struct s_simulation t_simulation;
typedef struct s_philo t_philo;

typedef struct s_parameters {
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_meals;
} t_parameters;

struct s_simulation {
    uint32_t num_philos;
    uint32_t time_to_eat;
    uint32_t time_to_sleep;
    uint32_t time_to_die;
    uint32_t num_meals;
    uint64_t start_time_stamp;
    pthread_t *threads;
    pthread_mutex_t *forks;
    pthread_mutex_t write_lock;
    t_philo *philos;
    volatile uint8_t running;
    pthread_mutex_t pf_lock;
    uint32_t philos_full;
};

struct s_philo {
    int philo_id;
    uint64_t last_meal;
    t_simulation *sim;
    uint32_t meal_count;
};

uint64_t get_time_ms();
void sim_delay(uint32_t time, t_simulation* sim);
void sim_trigger_event(enum EVENT type, t_philo *p);
void sim_run(int num_philos, int time_to_eat, int time_to_sleep,
             int time_to_die, int num_meals);
void* big_brother_routine(void *arg);
void* sim_run_big_brother(void *arg);