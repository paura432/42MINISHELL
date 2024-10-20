/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:11:17 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/10/20 21:56:53 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_sqrt(int nb)
{
	int	i;

	i = 0;
	if (nb * nb <= 2147483647)
	{
		while (i * i < nb)
		{
			i++;
			if (i * i > nb)
				return (i - 1);
		}
	}
	return (i);
}
