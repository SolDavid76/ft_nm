/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:12:40 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:22:41 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

extern int g_flags;
extern int g_return;

void ft_error(char *str, char *file, char *error)
{
	write(2, str, ft_strlen(str));
	write(2, file, ft_strlen(file));
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	g_return++;
}

int hexalen(int value)
{
	int res = 1;

	while (value >= 16)
	{
		value /= 16;
		res++;
	}
	return (res);
}

void ft_puthex(int nb)
{
	char base[] = "0123456789abcdef";

	if (nb < ft_strlen(base))
		write(1, &base[nb], 1);
	else
	{
		ft_puthex(nb / ft_strlen(base));
		ft_puthex(nb % ft_strlen(base));
	}
}

void print_symbol(t_symbol *symbol, int padding)
{
	if (!symbol->is_undef)
	{
		for (int j = hexalen(symbol->address); j < padding; j++)
			write(1, "0", 1);
		ft_puthex(symbol->address);
	}
	else
		write(1, "                ", padding);
	write(1, " ", 1);
	write(1, &symbol->type, 1);
	write(1, " ", 1);
	write(1, symbol->name, ft_strlen(symbol->name));
	write(1, "\n", 1);
}

void print_symbols(t_symbol *tab, int tabSize, int padding)
{
	for (int i = 0; i < tabSize - 1; i++)
	{
		if (g_flags & PRINT_UNDEF)
		{
			if ((tab[i].type == 'U' || tab[i].type == 'v' || tab[i].type == 'w') && tab[i].name[0] != '.')
				print_symbol(&tab[i], padding);
		}
		else if (g_flags & PRINT_EXTERN)
		{
			if ((('A' <= tab[i].type && tab[i].type <= 'Z') || tab[i].type == 'v' || tab[i].type == 'w') && tab[i].name[0] != '.')
				print_symbol(&tab[i], padding);
		}
		else if (g_flags & PRINT_ALL)
		{
			print_symbol(&tab[i], padding);
		}
		else
		{
			if (tab[i].type != 'a' && tab[i].name[0] != '.')
				print_symbol(&tab[i], padding);
		}
	}
}