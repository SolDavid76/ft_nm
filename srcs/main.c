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

void handle_64(char *ptr)
{
	int 				ncmds;
	struct Elf64_Ehdr	header;

	header = (Elf64_Ehdr *) ptr;
	ncmds = header.e_phnum;
}

void nm(char *ptr)
{
	int magic;

	magic = *(int *)ptr;
	if (magic == 0x464c457f) // .ELF
		handle_64(ptr);
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