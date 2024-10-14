/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:59:02 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/10/15 00:44:57 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	int	where_envp(char *argc, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			if (!ft_strncmp(envp[i], argc, ft_strlen(envp[i]) -
					ft_strlen(ft_strchr(envp[i], '='))))
				return (i);
		}
		else
		{
			if (!ft_strncmp(envp[i], argc, ft_strlen(envp[i]) -
					ft_strlen(ft_strchr(envp[i], '\0'))))
				return (i);
		}
		i++;
	}
	return (0);
}

static	int	var_in_envp(char *argc, char **envp, int ij[2])
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

int	mini_export(t_prompt *prompt)
{
	int		ij[2];
	int		pos;
	char	**argc;
	char	*key;
	char	*value;
	char	*new_var;

	argc = ft_comprobar_prompt(prompt);
	if (!export_wrong(argc))
		return(1);
	if (ft_matrixlen(argc) == 1)
	{
		char  **env = prompt->envp;
		while (*env)
		{
			ft_putstr_fd("declare -x ", 1);
			show_export_env(*env);
			env++;
		}
		return (0);
	}
	if (ft_matrixlen(argc) >= 2)
	{
		ij[0] = 1;
		while (argc[ij[0]])
		{
			char *equal_sign = ft_strchr(argc[ij[0]], '=');
			if (equal_sign)
			{
				key = ft_substr(argc[ij[0]], 0, equal_sign - argc[ij[0]]);
				value = ft_strdup(equal_sign + 1);
			}
			else
			{
				key = ft_strdup(argc[ij[0]]);
				value = NULL;
			}
			pos = var_in_envp(key, prompt->envp, ij);
			if (pos == 1)
			{
				pos = where_envp(key, prompt->envp);
				free(prompt->envp[pos]);
				if (!value)
				{
					prompt->envp[pos] = ft_strdup(key);
				}
				else
				{
					prompt->envp[pos] = ft_strjoin(key, "=");
					prompt->envp[pos] = ft_strjoin(prompt->envp[pos], value);
				}
			}
			else if (!pos)
			{
				if (!value)
				{
					new_var = ft_strdup(key);
					prompt->envp = ft_extend_matrix(prompt->envp, new_var);
				}
				else
				{
					new_var = ft_strjoin(key, "=");
					new_var = ft_strjoin_free(new_var, value);
					prompt->envp = ft_extend_matrix(prompt->envp, new_var);
				}
				free(new_var);
			}
			free(key);
			free(value);
			ij[0]++;
		}
	}
	return (0);
}
