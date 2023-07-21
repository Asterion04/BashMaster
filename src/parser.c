/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 03:55:50 by ndiamant          #+#    #+#             */
/*   Updated: 2023/07/21 13:34:56 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bashmaster.h"

static int	create_redirection_token(t_bash *sh, t_list *to_add, int i)
{
	to_add = ft_init_token();
	to_add->value = sh->lexed[i];
	to_add->fd_in = 0;
	to_add->fd_out = 0;
	to_add->arguments = 0;
	if (sh->lexed[i][0] == '<')
	{
		if (sh->lexed[i][1] == '<')
			to_add->id = HERE_DOC_TOKEN;
		else
			to_add->id = RED_ENTRY_TOKEN;
	}
	else
	{
		if (sh->lexed[i][1] == '>')
			to_add->id = APPEND_TOKEN;
		else
			to_add->id = RED_EXIT_TOKEN;
	}
	ft_add_token(sh, to_add);
	i++;
	return (i);
}

static int	create_pipe_token(t_bash *sh, t_list *to_add, int i)
{
	to_add = ft_init_token();
	to_add->value = sh->lexed[i];
	to_add->fd_in = 0;
	to_add->fd_out = 0;
	to_add->arguments = 0;
	to_add->id = PIPE_TOK;
	ft_add_token(sh, to_add);
	i++;
	return (i);
}

static int	create_cmd_token(t_bash *sh, t_list *to_add, int i)
{
	int	j;

	j = i;
	to_add = ft_init_token();
	to_add->value = sh->lexed[i];
	to_add->fd_in = 0;
	to_add->fd_out = 0;
	while (sh->lexed[j][0] != '<' && sh->lexed[j][0] != '>'
		&& sh->lexed[j][0] != '|' && sh->lexed[j])
		j++;
	to_add->arguments = malloc (sizeof(char *) * j + 1);
	j = 0;
	i++;
	while (sh->lexed[i][0] != '<' && sh->lexed[i][0] != '>'
		&& sh->lexed[i][0] != '|' && sh->lexed[i])
	{
		to_add->arguments[j] = sh->lexed[i];
		i++;
		j++;
	}
	to_add->arguments[j] = 0;
	to_add->id = CMD_TOK;
	ft_add_token(sh, to_add);
	return (i);
}

void	parser(t_bash *sh)
{
	int		i;
	t_list	*to_add;

	i = 0;
	sh->first = NULL;
	while (sh->lexed[i])
	{
		if (sh->lexed[i][0] == '<' || sh->lexed[i][0] == '>')
			i = create_redirection_token(sh, to_add, i);
		else if (sh->lexed[i][0] == '|')
			i = create_pipe_token(sh, to_add, i);
		else
			i = create_cmd_token(sh, to_add, i);
	}
	ft_print_tokens(sh);
}