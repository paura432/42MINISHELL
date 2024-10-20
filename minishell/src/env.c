/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:59:02 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/10/20 21:54:33 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	where_envp(char *argc, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			if (!ft_strncmp(envp[i], argc, ft_strlen(envp[i])
					- ft_strlen(ft_strchr(envp[i], '='))))
				return (i);
		}
		else
		{
			if (!ft_strncmp(envp[i], argc, ft_strlen(envp[i])
					- ft_strlen(ft_strchr(envp[i], '\0'))))
				return (i);
		}
		i++;
	}
	return (0);
}

int	var_in_envp(char *argc, char **envp, int ij[2])
{
	ij[1] = 0;
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argc, ft_strlen(argc)))
			return (1);
		ij[1]++;
	}
	return (0);
}

int	mini_unset(t_prompt *prompt)
{
	char	**argc;
	char	*aux;
	int		ij[2];

	ij[0] = 0;
	argc = ((t_mini *)prompt->cmds->content)->full_cmd;
	if (ft_matrixlen(argc) >= 2)
	{
		while (argc[++ij[0]])
		{
			if (argc[ij[0]][ft_strlen(argc[ij[0]]) - 1] != '=')
			{
				aux = ft_strjoin(argc[ij[0]], "=");
				free(argc[ij[0]]);
				argc[ij[0]] = aux;
			}
			if (var_in_envp(argc[ij[0]], prompt->envp, ij))
				ft_matrix_replace_in(&prompt->envp, NULL, ij[1]);
		}
	}
	return (0);
}

void	update_existing_var(t_prompt *prompt, char *key, char *value, int pos)
{
	free(prompt->envp[pos]);
	if (value == NULL)
		prompt->envp[pos] = ft_strdup(key);
	else
	{
		prompt->envp[pos] = ft_strjoin(key, "=");
		prompt->envp[pos] = ft_strjoin_free(prompt->envp[pos], value);
	}
}

int	mini_echo(t_list *cmd)
{
	int		newline;
	int		i[2];
	char	**argv;
	t_mini	*node;

	i[0] = 0;
	i[1] = 0;
	newline = 1;
	node = cmd->content;
	argv = node->full_cmd;
	while (argv && argv[++i[0]])
	{
		if (!i[1] && !ft_strncmp(argv[i[0]], "-n", 2) && \
			(ft_countchar(argv[i[0]], 'n') == \
			(int)(ft_strlen(argv[i[0]]) - 1)))
			newline = 0;
		else
		{
			i[1] = 1;
			ft_putstr_fd(argv[i[0]], 1);
			if (argv[i[0] + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	return (write(1, "\n", newline) == 2);
}
