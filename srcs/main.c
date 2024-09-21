/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:21:43 by djanusz           #+#    #+#             */
/*   Updated: 2024/09/21 19:01:12 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

typedef struct s_symbol
{
	int		adress;
	char	type;
	char	*name;
}			t_symbol;

char getSymbolType(Elf64_Shdr *shdr, Elf64_Sym sym)
{
	char c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOTE)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY)
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && ('A' <= c && c <= 'Z'))
		c += 32;
	return c;
}

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

char *ft_strjoin(char c, char *str)
{
	char	*res;
	int		i;

	res = malloc(sizeof(char) * (ft_strlen(str) + 3));
	res[0] = c;
	res[1] = ' ';
	for (i = 0; str[i]; i++)
		res[i + 2] = str[i];
	res[i + 2] = '\0';
	return (res);
}

void ft_swap(t_symbol *ptr1, t_symbol *ptr2)
{
	t_symbol tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

int	ft_strcmp(char *str1, char *str2)
{
	int	i = 0;

	while (str1[i] && str1[i] == str2[i])
		i++;
	return (str1[i] - str2[i]);
}

t_symbol *handle_64_symbols_sort(t_symbol *tab, int tabSize)
{
	for (int i = 0; i < tabSize - 1; i++)
	{
		for (int j = i + 1; j < tabSize - 1; j++)
		{
			int k = 0, l = 0;
			while (tab[i].name[k] && tab[i].name[k] =='_')
				k++;
			while (tab[j].name[l] && tab[j].name[l] == '_')
				l++;

			if (('a' <= tab[i].name[k] && tab[i].name[k] <= 'z') && ('a' <= tab[j].name[l] && tab[j].name[l] <= 'z'))
			{
				if (ft_strcmp(tab[i].name + k, tab[j].name + l) > 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else if (('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z') && ('A' <= tab[j].name[l] && tab[j].name[l] <= 'Z'))
			{
				if (ft_strcmp(tab[i].name + k, tab[j].name + l) > 0)
					ft_swap(&tab[i], &tab[j]);
			}
			else
			{
				if ('A' <= tab[i].name[k] && tab[i].name[k] <= 'Z')
				{
					if (tab[i].name[k] + 32 >= tab[j].name[l])
						ft_swap(&tab[i], &tab[j]);
				}
				else
				{
					if (tab[i].name[k] > tab[j].name[l] + 32)
						ft_swap(&tab[i], &tab[j]);
				}
			}
		}
	}
	return (tab);
}

void handle_64_symbols(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Shdr *symTab)
{
	Elf64_Sym	*symbol = (void *)ptr + symTab->sh_offset;
	char		*strTable = (char *)(ptr + sections[symTab->sh_link].sh_offset);
	int			symTabSize = symTab->sh_size / symTab->sh_entsize;

	t_symbol	*tab = malloc(symTabSize * sizeof(t_symbol));

	for (int i = 1; i < symTabSize; i++)
	{
		tab[i - 1].adress = symbol[i].st_value;
		tab[i - 1].type = getSymbolType(sections, symbol[i]);
		tab[i - 1].name = strTable + symbol[i].st_name;
	}

	handle_64_symbols_sort(tab, symTabSize);

	for (int i = 0; i < symTabSize - 1; i++)
		printf("%x %c %s\n", tab[i].adress, tab[i].type, tab[i].name);
}

void handle_64(char *ptr)
{
	Elf64_Ehdr	*header = (Elf64_Ehdr *)ptr;
	Elf64_Shdr	*sections = (void *)ptr + header->e_shoff;

	for (unsigned long int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			handle_64_symbols(ptr, header, sections, sections + i);
			break ;
		}
	}
}

void nm(char *ptr)
{
	int magic = *(int *)ptr;

	if (magic == 0x464c457f) // .ELF
		handle_64(ptr);
	else
		printf("Is not an ELF\n");
}

int main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac != 2)
		printf("No args\n");
	else if ((fd = open(av[1], O_RDONLY)) < 0)
		printf("Can't open file\n");
	else if (fstat(fd, &buf) < 0)
		printf("Can't read the file\n");
	else if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) < 0)
		printf("mmap crash\n");
	else
	{
		nm(ptr);
		munmap(ptr, buf.st_size);
	}
	return (0);
}