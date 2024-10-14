/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:30:18 by pau               #+#    #+#             */
/*   Updated: 2024/10/15 00:32:04 by pau              ###   ########.fr       */
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