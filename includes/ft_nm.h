/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:47:22 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:19:53 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <elf.h>

#define NO_SORT      1  // 00001 = -p
#define REVERSE_SORT 2  // 00010 = -r
#define PRINT_ALL    4  // 00100 = -a
#define PRINT_EXTERN 8  // 01000 = -g
#define PRINT_UNDEF  16 // 10000 = -u

typedef struct s_symbol
{
	int  is_undef;
	int  address;
	char type;
	char *name;
	char *name_min;
}	t_symbol;

/* handle.c */
void		parse_flag(char **tab);
int			ft_nbFiles(char **tab);
void		nm(char *file, char *ptr);

/* handle32.c */
char		handle_32_type(Elf32_Shdr *shdr, Elf32_Sym sym);
void		handle_32_symbols(char *ptr, Elf32_Ehdr *header, Elf32_Shdr *sections, Elf32_Shdr *symTab);
void		handle_32(char *file, char *ptr);

/* handle64.c */
char		handle_64_type(Elf64_Shdr *shdr, Elf64_Sym sym);
void		handle_64_symbols(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Shdr *symTab);
void		handle_64(char *file, char *ptr);

/* sort.c */
t_symbol	*handle_symbols_sort(t_symbol *tab, int tabSize);
t_symbol	*handle_symbols_reverse_sort(t_symbol *tab, int tabSize);

/* print.c */
void		ft_error(char *str, char *file, char *error);
int			hexalen(int value);
void		ft_puthex(int nb);
void		print_symbol(t_symbol *symbol, int padding);
void		print_symbols(t_symbol *tab, int tabSize, int padding);

/* utils.c */
int			ft_strlen(char *str);
void		ft_swap(t_symbol *ptr1, t_symbol *ptr2);
char		*ft_strdup(char *str);
char		*minimalize(char *str);
int			ft_strcmp(char *s1, char *s2);
void		free_all(t_symbol *tab, int size);
