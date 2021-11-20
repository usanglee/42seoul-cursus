/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulee <ulee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 16:39:16 by hyospark          #+#    #+#             */
/*   Updated: 2021/11/20 19:27:15 by ulee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int  ft_cd(t_bundle *bundle)
{
	int ret_chdir;
	char *dir;
	t_token *next_token;

	if (bundle == NULL || bundle->token == NULL)
		return (FAIL);
	next_token = bundle->token->next;
	if (next_token && next_token->next)
	{
		printf("cd error\n");
		while (bundle->token && bundle->token->token_type != PIPE)
			bundle->token = bundle->token->next;
		return (FAIL);
	}
	if (next_token == NULL || next_token->token_type == PIPE)
		dir = "~";
	else
		dir = next_token->content;
	ret_chdir = chdir(dir);
	if (ret_chdir == -1)
	{
		printf("%s\n", strerror(errno));
		return (FAIL);
	}
	return (SUCCESS);
}