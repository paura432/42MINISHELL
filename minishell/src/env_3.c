/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:33:45 by pau               #+#    #+#             */
/*   Updated: 2024/10/15 00:34:44 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	export_wrong(char **argc)
{
	if (ft_matrixlen(argc) >= 2 && argc[1][0] == '-')
	{
		mini_perror(argc, INV_OPTION, NULL, 1);
		return (0);	
	}
	else if (!if_alfnum_(argc))
	{
		mini_perror(argc, NOT_VAL_IDENT, NULL, 1);
		return (0);	
	}
	return (1);
}

int	if_alfnum_(char **cmds)
{
	int	i;
	int	j;

	i = -1;
	while (cmds[++i])
	{
		j = -1;
		while (cmds[i][++j])
		{
			if (!((cmds[i][j] >= 'a' && cmds[i][j] <= 'z') ||
				(cmds[i][j] >= 'A' && cmds[i][j] <= 'Z') ||
				(cmds[i][j] >= '0' && cmds[i][j] <= '9') ||
				cmds[i][j] == '_' || cmds[i][j] == '=' ) ||
				(cmds[i][0] == '=') || (cmds[i][0] >= '0' &&
				cmds[i][0] <= '9'))
				return (0);
			if (cmds[i][j + 1] == '=')
				return (1);
		}
	}
	return (1);
}
