/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/05 16:12:14 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//t_context	g_all;

void	sign_ctrl_c(int sign)
{
	(void) sign;
}

int	check_line(char *input)
{
	if (!input)
	{
		printf("exit pas d'input\n"); //fr en sorte qu'il l'affiche sur la mm ligne 
		return (FAILURE);
	}
	if (input[0] != '\0')
	{
		add_history(input);
		return (SUCCESS);
	}
	else
		return (EMPTY);
}

// void	ft_waitpid(void)
// {
// 	int	i;
// 	int	pid;
// 	int	status;

// 	i = 0;
// 	while (i < g_all.nb_cmd) // ca va poser pb si pas de fork pour les builtin
// 	{
// 		pid = waitpid(g_all.pid[i], &status, 0);
// 		if (pid > 0)
// 			printf("Child process %d exited with status %d\n", pid, status);
// 		if (pid <= 0)
// 		{
// 			perror_void("Minishell: waitpid()");
// 			exit(1); // code d'erreur ok ? 
// 		}
// 		i++;
// 	}
// }

void	ft_waitpid(void)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	while (i < g_all.nb_cmd) // ca va poser pb si pas de fork pour les builtin
	{
		pid = waitpid(g_all.pid[i], &status, 0);
		// if (pid > 0)
		// 	printf("Child process %d exited with status %d\n", pid, status);
		if (pid <= 0)
		{
			perror_void("Minishell: waitpid()");
			exit(1); // code d'erreur ok ? 
		}
		i++;
	}
}

int	env_update(char **envp)
{
	int	i;
	int	line;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			line = i;
			break ;
		}
		i++;
	}
	g_all.all_path = ms_split(envp[i] + 5, ':');
	if (!g_all.all_path)
		return (FAILURE);
	//printf_strs(g_all.all_path, WITH_INDEX, STDOUT_FILENO);//********
	return (SUCCESS);
}


void	minishell(char *input)
{
	t_cmd	*buf;
		
	if (lexer(input) == FAILURE)
		return ;
	if (parser() == FAILURE)
		return ;
	if (fill_t_cmd() == FAILURE)
		return ;
	g_all.pid = (pid_t *)malloc(sizeof(pid_t) * g_all.nb_cmd);
	if (!g_all.pid)
		return(perror_void("Minishell: malloc()"));
	buf = g_all.cmd;
	while (buf)
	{
		if (pipe(g_all.end) < 0)
			return (perror_void("Minishell: pipe()"));
		if (execute(buf) == FAILURE)
			return ;
		buf = buf->next;
	}
	ft_waitpid(); // ok ici le waitpid ? 
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	int			line_ok;

	init_global(ac, av, env);
	printf("pid:%d\n", getpid());//******
	while (1)
	{
		signal(SIGINT, sign_ctrl_c);
		if (env_update(env) == FAILURE) // MODIFIER
			break ;
		//env_pwd_update(); // faut pas mettre ca apres l'exec ?
		input = readline(WATERMELON "Minishell " WHITE);
		line_ok = check_line(input);
		if (line_ok == FAILURE)
			break ;
		if (line_ok != EMPTY)
		{
			g_all.nb_cmd = 1;
			minishell(input);
			free_all_lexer_and_cmd();
		}
		free(input);
	}
	close(g_all.fd_stdin);
	close(g_all.fd_stdout);
	//free les trucs de la globale 
	return (0);
}
