/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:21:43 by djanusz           #+#    #+#             */
/*   Updated: 2024/09/12 14:35:14 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

char print_type(Elf64_Shdr *shdr, Elf64_Sym sym)
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

char handle_64_letter(Elf64_Shdr *sections, Elf64_Sym *symbol, int index)
{
	// int	min = (ELF64_ST_BIND(symbol->st_info) == STB_GLOBAL) * 32;

	// if (symbol->st_shndx == SHN_UNDEF)
	// 	return ('U');
	// else if (symbol->st_shndx == SHN_ABS)
	// 	return ('a' + min);
	// else
	// 	return ('?');
	// if (symbol->st_shndx == SHN_UNDEF) {
	// 	return ('U' + min); // Symbole indéfini
	// } else if (symbol->st_shndx == SHN_ABS) {
	// 	return ('A' + min); // Symbole absolu
	// } else if (sections[index].sh_type == SHT_NOBITS && sections[index].sh_flags & SHF_ALLOC) {
	// 	return ('B' + min); // Symbole dans la section BSS (non initialisé)
	// } else if (sections[index].sh_type == SHT_PROGBITS && sections[index].sh_flags & SHF_EXECINSTR) {
	// 	return ('T' + min); // Symbole dans la section de texte (code)
	// } else if (sections[index].sh_type == SHT_PROGBITS && sections[index].sh_flags & SHF_WRITE) {
	// 	return ('D' + min); // Symbole dans la section de données (initialisé)
	// } else if (sections[index].sh_type == SHT_PROGBITS && !(sections[index].sh_flags & SHF_WRITE)) {
	// 	return ('R' + min); // Symbole en lecture seule (typiquement des constantes)
	// } else {
	// 	return ('?');
	// }
}

void handle_64_symbols(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Shdr *symTab)
{
	Elf64_Sym	*symbol = (void *)ptr + symTab->sh_offset;
	const char	*strTable = (const char *)(ptr + sections[symTab->sh_link].sh_offset);
	int			symTabSize = symTab->sh_size / symTab->sh_entsize;

	for (int i = 1; i < symTabSize; i++)
	{
		// fprintf(stderr, "DEBUG = %d", sections[symbol[i].st_shndx].sh_type);
		printf("%c %s\n", print_type(sections, symbol[i]), strTable + symbol[i].st_name);
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