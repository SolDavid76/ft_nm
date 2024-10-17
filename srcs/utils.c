/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:18:31 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:19:58 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

void ft_swap(t_symbol *ptr1, t_symbol *ptr2)
{
	t_symbol tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

char *ft_strdup(char *str)
{
	int   i = 0;
	char *res = malloc(sizeof(char) * (ft_strlen(str) + 1));

	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char *minimalize(char *str)
{
	int   i = 0;
	char *res = malloc(sizeof(char) * (ft_strlen(str) + 1));

	while (str[i])
	{
		if ('A' <= str[i] && str[i] <= 'Z')
			res[i] = str[i] + 32;
		else
			res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int ft_strcmp(char *s1, char *s2)
{
	int	i = 0, j = 0;

	while (s1[i] && s2[j] && (s1[i] == s2[j] || (s1[i] == '_' || s2[j] == '_') || (s1[i] == '@' || s2[j] == '@') || (s1[i] == '.' || s2[j] == '.')))
	{
		if ((s1[i] != '_' && s2[j] != '_') && (s1[i] != '@' && s2[j] != '@') && (s1[i] != '.' && s2[j] != '.'))
		{
			i++;
			j++;
		}
		else
		{
			if (s1[i] == '_' || s1[i] == '@' || s1[i] == '.')
				i++;
			if (s2[j] == '_' || s2[j] == '@' || s2[j] == '.')
				j++;
		}
	}
	return (s1[i] - s2[j]);
}

void free_all(t_symbol *tab, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		free(tab[i].name);
		free(tab[i].name_min);
	}
}
