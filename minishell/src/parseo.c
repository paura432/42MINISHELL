/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:14:27 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/09/10 22:18:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int g_status;

static	char	**split_all(char **argc, t_prompt *prompt)
{
	char	**subsplit;
	int		i;
	int		quotes[2];

	i = -1;
	while (argc && argc[++i])
	{
		argc[i] = expand_vars(argc[i], -1, quotes, prompt);
		argc[i] = expand_path(argc[i], -1, quotes, \
		mini_getenv("HOME", prompt->envp, 4));
		subsplit = ft_cmdsubsplit(argc[i], "<|>");
		ft_matrix_replace_in(&argc, subsplit, i);
		i += ft_matrixlen(subsplit) - 1;
		ft_free_matrix(&subsplit);	
	}
	return (argc);
}

static	void	*parse_args(char **argc, t_prompt *prompt)
{
	int	is_exit;
	int	i;

	is_exit = 0;
	prompt->cmds = fill_nodes(split_all(argc, prompt), -1);
	if (!prompt->cmds)
		return (prompt);
	i = ft_lstsize(prompt->cmds);
	g_status = builtin(prompt, prompt->cmds, &is_exit, 0);
	while (i-- > 0)
		waitpid(-1, &g_status, 0);
	if (!is_exit && g_status == 13)
		g_status = 0;
	if (g_status > 255)
		g_status = g_status / 255;
	if (argc && is_exit)
	{
		ft_lstclear(&prompt->cmds, free_content);
		return (NULL);
	}
	return (prompt);
}

void	*check_args(char *out, t_prompt *prompt)
{
	char	**a;
	t_mini	*n;

	if (!out)
		return (printf("exit\n"), NULL);
	if (out[0] != '\0')
		add_history(out);
	a = ft_cmdtrim(out, " ");
	free(out);
	if (!a)
		mini_perror(NULL, QUOTE, NULL, 1);
	if (!a)
		return ("");
	prompt = parse_args(a, prompt);
	if (prompt && prompt->cmds)
		n = prompt->cmds->content;
	if (prompt && prompt->cmds && n && n->full_cmd
		&& ft_lstsize(prompt->cmds) == 1)
	{
		prompt->envp = mini_setenv("_", n->full_cmd
			[ft_matrixlen(n->full_cmd) - 1], prompt->envp, 1);
	}
	if (prompt && prompt->cmds)
		ft_lstclear(&prompt->cmds, free_content);
	return (prompt);
}
