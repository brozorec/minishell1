/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/20 15:50:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/02/25 16:23:12 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

char		**get_reserved_paths(void)
{
	int			fd;
	char		*line;
	char		**tab;

	tab = (char **)malloc(2 * sizeof(char *));
	tab[0] = ft_strdup("PATH=");
	tab[1] = 0;
	if ((fd = open("/etc/paths", O_RDONLY)) == -1)
		fd = open("./source/builtins/path", O_RDONLY);
	while (fd != -1 && get_next_line(fd, &line) != 0)
	{
		tab[0] = ft_realloc(tab[0], ft_strlen(line) + 1);
		tab[0] = ft_strcat(tab[0], line);
		tab[0] = ft_strcat(tab[0], ":");
	}
	return (tab);
}

char		*dir_content(char *path, char *name)
{
	DIR					*dirp;
	struct dirent		*entry;
	char				*my_path;

	if ((dirp = opendir(path)) != 0)
	{
		while ((entry = readdir(dirp)) != 0)
		{
			if (ft_strcmp(entry->d_name, name) == 0)
			{
				my_path = ft_str3join(path, "/", name);
				closedir(dirp);
				return (my_path);
			}
		}
	}
	if (dirp)
		closedir(dirp);
	return (0);
}

char		**get_paths(char *var, char **env)
{
	int			i;
	int			cmp;

	i = 0;
	cmp = ft_strlen(var);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], var, cmp) == 0)
			return (ft_strsplit(&env[i][cmp], ':'));
		++i;
	}
	return (0);
}

char		*lookup_paths(char *var, char *name, char **env)
{
	int			i;
	char		**tab_paths;
	char		*my_path;

	i = 0;
	tab_paths = 0;
	tab_paths = get_paths(var, env);
	while (tab_paths && tab_paths[i])
	{
		if ((my_path = dir_content(tab_paths[i], name)) != 0)
		{
			ft_strdel(tab_paths);
			return (my_path);
		}
		i++;
	}
	if (tab_paths)
		ft_strdel(tab_paths);
	return (0);
}
