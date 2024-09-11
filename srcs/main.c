/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:21:43 by djanusz           #+#    #+#             */
/*   Updated: 2024/09/10 22:02:14 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

char handle_64_letter(Elf64_Sym *symbol)
{
	int	maj = (ELF64_ST_BIND(symbol->st_info) == STB_GLOBAL) * 32;

	if (symbol->st_shndx == SHN_UNDEF)
		return ('U');
	else if (symbol->st_shndx == SHN_ABS)
		return ('a' + maj);
	else
		return ('?');
}

void handle_64_symbols(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Shdr *symTab)
{
	Elf64_Sym	*symbol = (void *)ptr + symTab->sh_offset;
	const char	*strTable = (const char *)(ptr + sections[symTab->sh_link].sh_offset);
	int			symTabSize = symTab->sh_size / symTab->sh_entsize;

	for (int i = 1; i < symTabSize; i++)
	{
		// fprintf(stderr, "DEBUG = %4x ", symbol[i].st_shndx);
		printf("%c %s\n", handle_64_letter(symbol + i), strTable + symbol[i].st_name);
	}
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
	int magic;

	magic = *(int *)ptr;
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