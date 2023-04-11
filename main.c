/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/11 11:43:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_line(char *input)
{
	if (!input)
	{
		rl_replace_line("", 0); //replace the current contents of the Readline input buffer with a new string. 
		rl_on_new_line();
		rl_redisplay();
		printf("exit\n");
		// faire un join de input + exit 
		// enlever le /n de input
		//printf("%s\n", ms_strjoin(input, "exit")); //fr en sorte qu'il l'affiche sur la mm ligne 
		// faire un truc propre sinon ca ca fait des leaks 
		// printf("exit pas d'input\n"); //fr en sorte qu'il l'affiche sur la mm ligne 
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
		if (pid > 0)
		{
			if (WIFEXITED(status))
			{
				g_all.status = WEXITSTATUS(status);
				//printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));//****
			}
			else if (WIFSIGNALED(g_all.status))
			{
				if (g_all.status != 130)
					g_all.status = WTERMSIG(status);
				//printf("Child process %d terminated by signal %d\n", pid, WTERMSIG(status));//****
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

int	update_env_after_son(void)
{
	char	*pwd;

	if (g_all.is_first_turn == NO)
	{
		pwd = create_var_value("PWD", 0);
		if (!pwd)
			return (FAILURE);
		if (chdir(pwd) == -1)
		{
			perror("Minishell");
			free(pwd);
			return (FAILURE);
		}
		free(pwd);
	}
	return (SUCCESS);
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
		if (g_all.is_first_turn == NO && update_global() == FAILURE)
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
	//g_all.where = DAD;
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
	printf("pid:%d\n", getpid());//****** A SUPPRIMER
	while (1)
	{
		signal(SIGINT, signal_main);
		signal(SIGQUIT, SIG_IGN);
		//system("leaks minishell");
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
	//free les trucs de la globale 
	// close herit + size
	return (0);
}


