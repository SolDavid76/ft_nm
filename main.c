/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:21:43 by djanusz           #+#    #+#             */
/*   Updated: 2024/09/26 15:44:59 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

#define NO_SORT      1  // 00001 = -p
#define REVERSE_SORT 2  // 00010 = -r
#define PRINT_ALL    4  // 00100 = -a
#define PRINT_EXTERN 8  // 01000 = -g
#define PRINT_UNDEF  16 // 10000 = -u

typedef struct s_symbol
{
	int  adress;
	char type;
	char *name;
	char *name_min;
}			t_symbol;

int flags = 0;

char getSymbolType(Elf64_Shdr *shdr, Elf64_Sym sym)
{
	char c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
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
	char *res;
	int  i;

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
	char *res = malloc(sizeof(char) * ft_strlen((str) + 1));

	for (int i = 0; str[i]; i++)
	{
		if ('A' <= str[i] && str[i] <= 'Z')
			res[i] = str[i] + 32;
		else
			res[i] = str[i];
	}
	return (res);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int	i = 0, j = 0;

    while (s1[i] && s2[j] && (s1[i] == s2[j] || (s1[i] == '_' || s2[j] == '_')))
    {
        if (s1[i] != '_' && s2[j] != '_') 
        {
            i++;
            j++;
        }
        else
        {
            if (s1[i] == '_')
                i++;
            if (s2[j] == '_')
                j++;
        }
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[j]);
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

			if (ft_strcmp(tab[i].name, tab[j].name) == 0 && tab[i].type == tab[j].type && tab[i].adress > tab[j].adress)
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

void ft_putnbr_base(unsigned int nb, char *base)
{
	if (nb < ft_strlen(base))
		write(1, &base[nb], 1);
	else
	{
		ft_putnbr_base(nb / ft_strlen(base), base);
		ft_putnbr_base(nb % ft_strlen(base), base);
	}
}

void print_64_symbol(t_symbol *symbol)
{
	if (symbol->type != 'U' && (symbol->adress != 0 || symbol->type == 'a'))
	{
		for (int j = hexalen(symbol->adress); j < 16; j++)
			write(1, "0", 1);
		ft_putnbr_base(symbol->adress, "0123456789abcdef");
	}
	else
		write(1, "                ", 16);
	write(1, " ", 1);
	write(1, &symbol->type, 1);
	write(1, " ", 1);
	write(1, symbol->name, ft_strlen(symbol->name));
	write(1, "\n", 1);
}

void print_64_symbols(t_symbol *tab, int tabSize)
{
	if (!(flags & REVERSE_SORT))
	{
		for (int i = 0; i < tabSize - 1; i++)
		{
			if (flags & PRINT_ALL)
				print_64_symbol(&tab[i]);
			else if (flags & PRINT_EXTERN)
			{
				if (('A' <= tab[i].type && tab[i].type <= 'Z') || tab[i].type == 'u' || tab[i].type == 'v' || tab[i].type == 'w')
					print_64_symbol(&tab[i]);
			}
			else if (flags & PRINT_UNDEF)
			{
				if (tab[i].type == 'U' || tab[i].type == 'v' || tab[i].type == 'w')
					print_64_symbol(&tab[i]);
			}
			else
			{
				if (tab[i].type != 'a')
					print_64_symbol(&tab[i]);
			}
		}
	}
	else
	{
		for (int i = tabSize - 1 ; i > 0; i--)
		{
			if (flags & PRINT_ALL)
				print_64_symbol(&tab[i]);
			else if (flags & PRINT_EXTERN)
			{
				if (('A' <= tab[i].type && tab[i].type <= 'Z') || tab[i].type == 'u' || tab[i].type == 'v' || tab[i].type == 'w')
					print_64_symbol(&tab[i]);
			}
			else if (flags & PRINT_UNDEF)
			{
				if (tab[i].type == 'U' || tab[i].type == 'v' || tab[i].type == 'w')
					print_64_symbol(&tab[i]);
			}
			else
			{
				if (tab[i].type != 'a')
					print_64_symbol(&tab[i]);
			}
		}
	}
}

void handle_64_symbols(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Shdr *symTab)
{
	Elf64_Sym *symbol = (void *)ptr + symTab->sh_offset;
	char      *strTable = (char *)(ptr + sections[symTab->sh_link].sh_offset);
	int        symTabSize = symTab->sh_size / symTab->sh_entsize;

	t_symbol  *tab = malloc(symTabSize * sizeof(t_symbol));

	for (int i = 1; i < symTabSize; i++)
	{
		tab[i - 1].adress = symbol[i].st_value;
		tab[i - 1].type = getSymbolType(sections, symbol[i]);
		tab[i - 1].name = ft_strdup(strTable + symbol[i].st_name);
		tab[i - 1].name_min = minimalize(tab[i - 1].name);
	}

	if (!(flags & NO_SORT))
		handle_64_symbols_sort(tab, symTabSize);

	print_64_symbols(tab, symTabSize);

	//free tab here
	exit(0);
}

void handle_64(char *ptr)
{
	Elf64_Ehdr *header = (Elf64_Ehdr *)ptr;
	Elf64_Shdr *sections = (void *)ptr + header->e_shoff;

	for (unsigned long int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			handle_64_symbols(ptr, header, sections, sections + i);
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

int ft_nbFiles(char **tab)
{
	int res = 0;

	for (int i = 0; tab[i]; i++)
	{
		if (tab[i][0] != '-')
			res++;
	}
	return (res);
}

void parse_flag(char **tab)
{
	for (int i = 0; tab[i]; i++)
	{
		for (int j = 1; tab[i][0] == '-' && tab[i][j]; j++)
		{
			if (tab[i][j] == 'p')
				flags |= NO_SORT;
			else if (tab[i][j] == 'r')
				flags |= REVERSE_SORT;
			else if (tab[i][j] == 'a')
				flags |= PRINT_ALL;
			else if (tab[i][j] == 'g')
				flags |= PRINT_EXTERN;
			else if (tab[i][j] == 'u')
				flags |= PRINT_UNDEF;
			else
			{
				write(2, "ft_nm: invalid option -- '", 26);
				write(2, &tab[i][j], 1);
				write(2, "'\n", 2);
				write(2, "Usage: ft_nm [option(s)] [file(s)]\n", 35);
				write(2, "List symbols in [file(s)] (a.out by default).\n", 46);
				write(2, "The options are:\n", 18);
				write(2, "-a,                    Display debugger-only symbols\n", 53);
				write(2, "-g,                    Display only external symbols\n", 53);
				write(2, "-p,                    Do not sort the symbols\n", 47);
				write(2, "-r,                    Reverse the sense of the sort\n", 53);
				write(2, "-u,                    Display only undefined symbols\n", 54);
				exit(1);
			}
		}
	}
}

int main(int ac, char **av)
{
	int         fd;
	char       *ptr;
	struct stat buf;

	int         nbFiles = ft_nbFiles(av + 1);

	parse_flag(av + 1);

	if (nbFiles == 0)
		main(ac + 1, (char *[]){av[0], "a.out", NULL});
	else
	{
		for (int i = 1; i < ac; i++)
		{
			if (av[i][0] != '-')
			{
				if ((fd = open(av[i], O_RDONLY)) < 0)
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
			}
		}
	}
	return (0);
}
