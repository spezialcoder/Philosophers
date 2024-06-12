/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:25:46 by lsorg             #+#    #+#             */
/*   Updated: 2024/06/12 15:59:43 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

void* my_thread(void *idx) {
	printf("Thread: %d\n",*((int*)idx));
	return NULL;
}

int main() {
	pthread_t handle;
	pthread_t handle2;
	pthread_t handle3;
	pthread_t handle4;
	pthread_create(&handle, NULL, my_thread, (int[]){1});
	pthread_create(&handle2, NULL, my_thread, (int[]){2});
	pthread_create(&handle3, NULL, my_thread, (int[]){3});
	pthread_create(&handle4, NULL, my_thread, (int[]){4});
	pthread_join(handle,NULL);
	pthread_join(handle2,NULL);
	pthread_join(handle3,NULL);
	pthread_join(handle4,NULL);

	return 0;
}