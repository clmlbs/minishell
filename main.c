/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 10:43:46 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_line(char *input)
{
	if (!input)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		printf("exit\n");
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

void	ft_waitpid(void)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	while (i < g_all.nb_cmd)
	{
		pid = waitpid(g_all.pid[i], &status, 0);
		if (pid > 0)
		{
			if (WIFEXITED(status))
				g_all.status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_all.status = 128 + WTERMSIG(status);
				if (g_all.status == 131)
					printf("Quit: 3\n");
				else if (WTERMSIG(status) == SIGSEGV)
					ft_putstr_fd("Segmentation fault: 11\n", STDERR_FILENO);
				else if (WTERMSIG(status) == SIGBUS)
					ft_putstr_fd("Bus error: 10\n", STDERR_FILENO);	
				else if (WTERMSIG(status) == SIGABRT)
					ft_putstr_fd("Abort trap: 6\n", STDERR_FILENO);
				else if (WTERMSIG(status) == SIGTERM)
					ft_putstr_fd("Terminated: 15\n", STDERR_FILENO);	
				else if (WTERMSIG(status) == SIGKILL)
					ft_putstr_fd("Killed: 9\n", STDERR_FILENO);	
			}
		}
		if (pid <= 0)
		{
			perror_void("Minishell: waitpid()");
			exit(FAILURE); // code d'erreur ok ? 
		}
		i++;
	}
}

int	replace_dollar_question_mark(char **strs)
{
	int		i;
	char	*status;

	i = 0;
	status = ft_itoa(g_all.status);
	if (!status)
		return (FAILURE);
	while (strs && strs[i])
	{
		if (ms_strlen(strs[i]) == 2 && !ft_strncmp(strs[i], "$?", 2))
		{
			free(strs[i]);
			strs[i] = ft_strdup(status);
			if (!strs[i])
			{
				free(status);
				return (FAILURE);
			}
		}
		i++;
	}
	free(status);
	return (SUCCESS);
}

int	execute_line(void)
{
	t_cmd	*buf;

	g_all.pid = (pid_t *)malloc(sizeof(pid_t) * g_all.nb_cmd);
	if (!g_all.pid)
		return (perror_fail("Minishell: malloc()"));
	buf = g_all.cmd;
	while (buf)
	{
		if (g_all.is_first_turn == NO && isatty(STDIN_FILENO) \
			&& update_global() == FAILURE)
			return (FAILURE);
		else
		{
			if (pipe(g_all.end) < 0)
				return (perror_fail("Minishell: pipe()"));
			if (replace_dollar_question_mark(buf->wd) == FAILURE \
				|| execute(buf) == FAILURE)
				return (FAILURE);
			buf = buf->next;
		}
	}
	ft_waitpid(); // ok ici le waitpid ?
	if (g_all.pid)
	{
		free(g_all.pid);
		g_all.pid = NULL;
	}
	g_all.where = PROCESS;
	return (SUCCESS);
}

void	minishell(char *input)
{
	if (lexer(input) == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
	if (parser() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 258;
		return ;
	}
	if (fill_t_cmd() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
	if (execute_line() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
}

void	free_everything(void)
{
	close(g_all.fd_stdin);
	close(g_all.fd_stdout);
	close(g_all.herit[0]);
	close(g_all.herit[1]);
	if (g_all.all_path)
		free_tab_strs(g_all.all_path);
	if (g_all.env)
		free_tab_strs(g_all.env);
	if (g_all.tilde)
		free(g_all.tilde);
	if (g_all.pid)
		free(g_all.pid);
	// autres trucs a free ?
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	int			line_ok;

	init_global(ac, av, env);
	if (pipe(g_all.herit) < 0)
		return (perror_fail("Minishell: pipe()"));
	printf("1er pid:%d\n", getpid());//****** A SUPPRIMER
	while (1)
	{
		system("leaks minishell"); //********
		echo_ctl(0);
		signal(SIGINT, signal_handle);
		signal(SIGQUIT, signal_handle);
		if (g_all.is_first_turn == NO && isatty(STDIN_FILENO) \
			&& update_global() == FAILURE) // rajout mais pb avec system leak;
			return (FAILURE);
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
		g_all.is_first_turn = NO;
	}
	free_everything();
	close(g_all.fd_stdin);
	close(g_all.fd_stdout);
	system("leaks minishell"); //********
	//free les trucs de la globale 
	// close herit + size
	return (0);
}


