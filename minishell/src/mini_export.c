/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pau <pau@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:37:05 by pau               #+#    #+#             */
/*   Updated: 2024/10/20 21:54:59 by pau              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	g_status;

void	display_export(t_prompt *prompt)
{
	char	**env;

	env = prompt->envp;
	while (*env)
	{
		ft_putstr_fd("declare -x ", 1);
		show_export_env(*env);
		env++;
	}
}

void	extract_key_value(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

void	add_new_var(t_prompt *prompt, char *key, char *value)
{
	char	*new_var;

	if (value == NULL)
		new_var = ft_strdup(key);
	else
	{
		new_var = ft_strjoin(key, "=");
		new_var = ft_strjoin_free(new_var, value);
	}
	prompt->envp = ft_extend_matrix(prompt->envp, new_var);
	free(new_var);
}

int	mini_export(t_prompt *prompt)
{
	char	**argc;
	int		ij[2];

	argc = ft_comprobar_prompt(prompt);
	if (!export_wrong(argc))
		return (1);
	if (ft_matrixlen(argc) == 1)
	{
		display_export(prompt);
		return (0);
	}
	ij[0] = 1;
	while (argc[ij[0]])
	{
		handle_export_argument(prompt, argc[ij[0]], ij);
		ij[0]++;
	}
	return (0);
}

void	handle_export_argument(t_prompt *prompt, char *arg, int ij[2])
{
	int		pos;
	char	*key;
	char	*value;

	extract_key_value(arg, &key, &value);
	pos = var_in_envp(key, prompt->envp, ij);
	if (pos == 1)
	{
		pos = where_envp(key, prompt->envp);
		update_existing_var(prompt, key, value, pos);
	}
	else
	{
		add_new_var(prompt, key, value);
	}
	free(key);
	free(value);
}
