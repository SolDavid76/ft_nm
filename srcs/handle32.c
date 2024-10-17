/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:06:37 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:22:41 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

extern int g_flags;

char handle_32_type(Elf32_Shdr *shdr, Elf32_Sym sym)
{
	char c;

	if (ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK && ELF32_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK)
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
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR | SHF_GROUP)))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_GROUP) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_MERGE | SHF_STRINGS)))
		c = 'R';
	else if ((shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == 0) || shdr[sym.st_shndx].sh_flags == (SHF_MERGE | SHF_STRINGS))
		c = 'N';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOTE)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
		c = 'D';
	else
		c = '?';
	if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && ('A' <= c && c <= 'Z') && c != 'N')
		c += 32;
	return c;
}

void handle_32_symbols(char *ptr, Elf32_Ehdr *header, Elf32_Shdr *sections, Elf32_Shdr *symTab)
{
	Elf32_Sym *symbol = (void *)ptr + symTab->sh_offset;
	char      *strTable = (char *)(ptr + sections[symTab->sh_link].sh_offset);
	char      *shStrTable = (char *)(ptr + sections[header->e_shstrndx].sh_offset);
	int        symTabSize = symTab->sh_size / symTab->sh_entsize;

	t_symbol  *tab = malloc(symTabSize * sizeof(t_symbol));

	for (int i = 1; i < symTabSize; i++)
	{
		tab[i - 1].is_undef = symbol[i].st_shndx == SHN_UNDEF;
		tab[i - 1].address = symbol[i].st_value;
		tab[i - 1].type = handle_32_type(sections, symbol[i]);
		if (symbol[i].st_info == STT_SECTION)
			tab[i - 1].name = ft_strdup(shStrTable + sections[symbol[i].st_shndx].sh_name);
		else
			tab[i - 1].name = ft_strdup(strTable + symbol[i].st_name);
		tab[i - 1].name_min = minimalize(tab[i - 1].name);
	}

	if (!(g_flags & NO_SORT))
	{
		if(!(g_flags & REVERSE_SORT))
			handle_symbols_sort(tab, symTabSize);
		else
			handle_symbols_reverse_sort(tab, symTabSize);
	}
	print_symbols(tab, symTabSize, 8);

	free_all(tab, symTabSize);
}

void handle_32(char *file, char *ptr)
{
	Elf32_Ehdr *header = (Elf32_Ehdr *)ptr;
	Elf32_Shdr *sections = (void *)ptr + header->e_shoff;

	for (unsigned long int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			handle_32_symbols(ptr, header, sections, sections + i);
			return;
		}
	}
	ft_error("ft_nm: ", file, ": no symbols");
}
