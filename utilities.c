/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:19:55 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 13:46:49 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	if (!str)
		return (0);
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else
	{
		sign = 1;
	}
	while (*str)
	{
		result *= 10;
		result += *str - '0';
		str++;
	}
	return (result * sign);
}
