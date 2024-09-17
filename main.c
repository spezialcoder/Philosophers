/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:25:46 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:51:53 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosim.h"
#include "utilities.h"

int	main(int argc, char *argv[])
{
	t_parameters	p;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo <number of philosophers> <time to die> "
			"<time to eat> <time to sleep> <?number of meals?>\n");
		return (0);
	}
	p = (t_parameters){ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]),
		ft_atoi(argv[4]), 0};
	if (argc == 6)
		p.num_meals = ft_atoi(argv[5]);
	sim_run(p);
	return (0);
}
