/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:59:02 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/10/14 23:51:20 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*mini_getenv(char *var, char **envp, int n)
{
	int	i;
	int	n2;

	i = 0;
	if (n < 0)
		n = ft_strlen(var);
	while (!ft_strchr(var, '=') && envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_i(envp[i], '='))
			n2 = ft_strchr_i(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
			return (ft_substr(envp[i], n2 + 1, ft_strlen(envp[i])));
		i++;
	}
	return (NULL);
}

char	**mini_setenv(char *var, char *value, char **envp, int n)
{
	int		i[2];
	char	*aux[2];

	if (n < 0)
		n = ft_strlen(var);
	i[0] = -1;
	aux[0] = ft_strjoin(var, "=");
	aux[1] = ft_strjoin(aux[0], value);
	free(aux[0]);
	while (!ft_strchr(var, '=') && envp && envp[++i[0]])
	{
		i[1] = n;
		if (i[1] < ft_strchr_i(envp[i[0]], '='))
			i[1] = ft_strchr_i(envp[i[0]], '=');
		if (!ft_strncmp(envp[i[0]], var, i[1]))
		{
			aux[0] = envp[i[0]];
			envp[i[0]] = aux[1];
			free(aux[0]);
			return (envp);
		}
	}
	envp = ft_extend_matrix(envp, aux[1]);
	free(aux[1]);
	return (envp);
}

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

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = strlen(s1);
	len2 = strlen(s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	strcpy(result, s1);
	strcat(result, s2);
	free(s1);
	return (result);
}

void	show_export_env(char *str)
{
	int	i;
	int	lock;

	i = 0;
	lock = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		if (str[i] == '=' && lock == 0)
		{
			write(1, "\"", 1);
			lock++;
		}
		i++;
	}
	if (lock != 0)
		write(1, "\"", 1);
	write(1, "\n", 1);
}

char	**ft_comprobar_prompt(t_prompt *prompt)
{
	t_list	*puntero;
	int		i;

	puntero = (t_list *)prompt->cmds;
	while (puntero->next)
	{
		if (!ft_strncmp(((t_mini *)puntero->content)->full_cmd[0], "export", 6))
			return (((t_mini *)puntero->content)->full_cmd);
		puntero = puntero->next;
	}
	if (!ft_strncmp(((t_mini *)puntero->content)->full_cmd[0], "export", 6))
		return (((t_mini *)puntero->content)->full_cmd);
	return (NULL);

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

int	mini_unset(t_prompt *prompt)
{
	char	**argc;
	char	*aux;
	int	ij[2];

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
