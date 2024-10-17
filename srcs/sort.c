/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:13:52 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:14:26 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_symbol *handle_symbols_sort(t_symbol *tab, int tabSize)
{
	for (int i = 0; i < tabSize - 1; i++)
	{
		for (int j = i + 1; j < tabSize - 1; j++)
		{
			int k = 0, l = 0;
			while (tab[i].name[k] && (tab[i].name[k] == '_' || tab[i].name[k] == '.'))
				k++;
			while (tab[j].name[l] && (tab[j].name[l] == '_' || tab[j].name[l] == '.'))
				l++;

			if (ft_strcmp(tab[i].name, tab[j].name) == 0 && tab[i].type == tab[j].type && tab[i].address > tab[j].address)
				ft_swap(&tab[i], &tab[j]);
			else if (ft_strcmp(tab[i].name, tab[j].name) == 0 && tab[i].type < tab[j].type)
				ft_swap(&tab[i], &tab[j]);
			else if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) == 0 && k < l)
				ft_swap(&tab[i], &tab[j]);
			else if (('a' <= tab[i].name[k] && tab[i].name[k] <= 'z') && ('a' <= tab[j].name[l] && tab[j].name[l] <= 'z'))
			{
				if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) > 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else if (('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z') && ('A' <= tab[j].name[l] && tab[j].name[l] <= 'Z'))
			{
				if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) > 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else
			{
				if ('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z')
				{
					if (tab[i].name[k] + 32 == tab[j].name[l] && ft_strcmp(tab[i].name_min + k + 1, tab[j].name_min + l + 1) > 0)
						ft_swap(&tab[i], &tab[j]);
					else if (tab[i].name[k] + 32 > tab[j].name[l])
						ft_swap(&tab[i], &tab[j]);
				}
				else
				{
					if (tab[i].name[k] == tab[j].name[l] + 32 && ft_strcmp(tab[i].name_min + k + 1, tab[j].name_min + l + 1) > 0)
						ft_swap(&tab[i], &tab[j]);
					else if (tab[i].name[k] > tab[j].name[l] + 32)
						ft_swap(&tab[i], &tab[j]);
				}
			}
		}
	}
	return (tab);
}

t_symbol *handle_symbols_reverse_sort(t_symbol *tab, int tabSize)
{
	for (int i = 0; i < tabSize - 1; i++)
	{
		for (int j = i + 1; j < tabSize - 1; j++)
		{
			int k = 0, l = 0;
			while (tab[i].name[k] && (tab[i].name[k] == '_' || tab[i].name[k] == '.'))
				k++;
			while (tab[j].name[l] && (tab[j].name[l] == '_' || tab[j].name[l] == '.'))
				l++;

			if (ft_strcmp(tab[i].name, tab[j].name) == 0 && tab[i].type == tab[j].type && tab[i].address > tab[j].address)
				ft_swap(&tab[i], &tab[j]);
			else if (ft_strcmp(tab[i].name, tab[j].name) == 0 && tab[i].type < tab[j].type)
				ft_swap(&tab[i], &tab[j]);
			else if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) == 0 && k > l)
				ft_swap(&tab[i], &tab[j]);
			else if (('a' <= tab[i].name[k] && tab[i].name[k] <= 'z') && ('a' <= tab[j].name[l] && tab[j].name[l] <= 'z'))
			{
				if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) < 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else if (('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z') && ('A' <= tab[j].name[l] && tab[j].name[l] <= 'Z'))
			{
				if (ft_strcmp(tab[i].name_min + k, tab[j].name_min + l) < 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else
			{
				if ('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z')
				{
					if (tab[i].name[k] + 32 == tab[j].name[l] && ft_strcmp(tab[i].name_min + k + 1, tab[j].name_min + l + 1) < 0)
						ft_swap(&tab[i], &tab[j]);
					else if (tab[i].name[k] + 32 < tab[j].name[l])
						ft_swap(&tab[i], &tab[j]);
				}
				else
				{
					if (tab[i].name[k] == tab[j].name[l] + 32 && ft_strcmp(tab[i].name_min + k + 1, tab[j].name_min + l + 1) < 0)
						ft_swap(&tab[i], &tab[j]);
					else if (tab[i].name[k] < tab[j].name[l] + 32)
						ft_swap(&tab[i], &tab[j]);
				}
			}
		}
	}
	return (tab);
}
