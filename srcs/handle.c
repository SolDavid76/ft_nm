/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:10:16 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:36:29 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

extern int g_flags;

void nm(char *file, char *ptr)
{
	int magic = *(int *)ptr;

	if (magic == 0x464c457f) // .ELF
	{
		if (ptr[4] == 1)
			handle_32(file, ptr);
		else if (ptr[4] == 2)
			handle_64(file, ptr);
	}
	else
		ft_error("ft_nm: ", file, ": file format not recognized");
}

int ft_nbFiles(char **tab)
{
	int res = 0;

	for (int i = 0; tab[i]; i++)
	{
		if (tab[i][0] != '-' || tab[i][1] == '\0')
			res++;
	}
	return (res);
}

void parse_flag(char **tab)
{
	for (int i = 0; tab[i]; i++)
	{
		for (int j = 1; tab[i][0] == '-' && (tab[i][1] != '-' || tab[i][2] == '-') && tab[i][j]; j++)
		{
			if (tab[i][j] == 'p')
				g_flags |= NO_SORT;
			else if (tab[i][j] == 'r')
				g_flags |= REVERSE_SORT;
			else if (tab[i][j] == 'a')
				g_flags |= PRINT_ALL;
			else if (tab[i][j] == 'g')
				g_flags |= PRINT_EXTERN;
			else if (tab[i][j] == 'u')
				g_flags |= PRINT_UNDEF;
			else
			{
				write(2, "ft_nm: invalid option -- '", 26);
				write(2, &tab[i][j], 1);
				write(2, "'\n", 2);
				write(2, "Usage: ft_nm [option(s)] [file(s)]\n", 35);
				write(2, "List symbols in [file(s)] (a.out by default).\n", 46);
				write(2, "The options are:\n", 18);
				write(2, "-a                     Display debugger-only symbols\n", 53);
				write(2, "-g                     Display only external symbols\n", 53);
				write(2, "-p                     Do not sort the symbols\n", 47);
				write(2, "-r                     Reverse the sense of the sort\n", 53);
				write(2, "-u                     Display only undefined symbols\n", 54);
				exit(1);
			}
		}
	}
}
