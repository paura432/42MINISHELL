/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpajuelo <jpajuelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:45:42 by jpajuelo          #+#    #+#             */
/*   Updated: 2024/08/20 15:15:56 by jpajuelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

static void	update_output(char ***matrix, int fd)
{
	char	**aux;
	char	*temp;
	char	*line;

	aux = NULL;
	line = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		temp = ft_strtrim(line, "\n");
		free(line);
		aux = ft_extend_matrix(aux, temp);
		free(temp);
	}
	ft_free_matrix(matrix);
	*matrix = aux;
}

void	exec_custom(char ***out, char *full, char *args, char **envp)
{
	pid_t	pid;
	int		fd[2];
	char	**matrix;

	pipe(fd);
	pid = fork();
	if (!pid)
	{
		close(fd[READ_END]);
		matrix = ft_split(args, ' ');
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		if (!access(full, F_OK))
			execve(full, matrix, envp);
		exit (1);
	}
	close(fd[WRITE_END]);
	waitpid(pid, NULL, 0);
	update_output(out, fd[READ_END]);
	close(fd[READ_END]);
}