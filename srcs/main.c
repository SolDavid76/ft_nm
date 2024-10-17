/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz   <djanusz  @student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:21:43 by djanusz           #+#    #+#             */
/*   Updated: 2024/10/17 13:36:15 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int g_flags = 0;
int g_return = 0;

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
			if (av[i] && (av[i][0] != '-' || av[i][1] == '\0'))
			{
				if ((fd = open(av[i], O_RDONLY)) < 0)
				{
					if (errno == 2)
						ft_error("ft_nm: '", av[i], "': No such file");
					else
						ft_error("ft_nm: ", av[i], ": Permission denied");
				}
				else if (fstat(fd, &buf) < 0)
					ft_error("ft_nm: ", av[i], ": Permission denied");
				else if (!S_ISREG(buf.st_mode))
					ft_error("ft_nm: Warning: '", av[i], "' is a directory");
				else if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == NULL)
					ft_error("ft_nm: ", av[i], ": Something went wrong");
				else
				{
					if (nbFiles > 1)
					{
						write(1, "\n", 1);
						write(1, av[i], ft_strlen(av[i]));
						write(1, ":\n", 2);
					}
					nm(av[i], ptr);
					munmap(ptr, buf.st_size);
				}
			}
		}
	}
	return (g_return);
}
