/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyospark <hyospark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 20:30:10 by hyospark          #+#    #+#             */
/*   Updated: 2021/12/02 16:56:19 by hyospark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **start_sh(char **env, char *input)
{
	t_bundle	*bundles;
	int			i;
	int			result;
	char		**return_env;

	if (is_space_str(input))
		return (NULL);
	bundles = split_bundle(env, input);
	if (parsing_token(bundles) == FAIL)
	{
		free_bundle(bundles);
		return (NULL);
	}
	i = 0;
	while (bundles[i].cmd_line)
	{
		result = execute_cmd(&bundles[i]);
		i++;
		if (bundles[i].cmd_line != NULL &&
		(result == SUCCESS && bundles[i].priority == P_OR) \
		|| (result == FAIL && bundles[i].priority == P_AND))
		{
			i++;
		}
	}
	return_env = bundles->env;
	free_bundle(bundles);
	return (return_env);
}

char	**dup_envp(char **envp)
{
	char	**env;
	int		len;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	env = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

void sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("%c[K\n", 27);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit: 3           ");
		rl_replace_line("                  ", 0);
		rl_redisplay();
		g_status = 131;
		printf("%c\n", 27);
	}
}

void	loop(char **env)
{
	char	*input;
	char	**dup_env;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	dup_env = dup_envp(env);
	while(TRUE)
	{
		input = readline("minishell$ ");
		add_history(input);
		dup_env = start_sh(env, input);
		free(input);
		if (env == NULL)
			env = dup_env;
	}
	free(env);
}

int main(int argc, char **av, char **envp)
{
	char **dup_env;
	char **dup_av;

	g_status = 0;
	dup_env = dup_envp(envp);
	loop(dup_env);
	return 0;
}
