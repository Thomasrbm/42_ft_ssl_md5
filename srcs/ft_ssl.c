#include "ft_ssl.h"

static const t_algo	g_algos[] = {
	{"md5", "MD5", ft_md5},
	{"sha256", "SHA256", ft_sha256},
	{NULL, NULL, NULL}
};

static char	*read_all(FILE *file)
{
	char	*final_buffer;
	char	reading_buf[4096];
	size_t	total_read;
	size_t	bytes_read;

	final_buffer = NULL;
	total_read = 0;
	while ((bytes_read = fread(reading_buf, 1, 4096, file)) > 0) // buf stock, taille de chaque element lu, max a lire, fd/*FILE
	{
		final_buffer = realloc(final_buffer, total_read + bytes_read + 1);     // agrandit final_buffer (+1 pour le \0)
		memcpy(final_buffer + total_read, reading_buf, bytes_read);            // copie apres ce que deja lue (total read)
		total_read += bytes_read;
	}
	if (!final_buffer)
		final_buffer = calloc(1, 1);   // pas de segfault quand ecrit dans final buff le \0 si fichier vide (sinon ecrit dans NULL)
	final_buffer[total_read] = '\0';
	return (final_buffer);
}

static void	hash_stdin(t_ssl *s)
{
	char	*input;
	char	*hash;
	size_t	len;

	input = read_all(stdin);
	hash = g_algos[s->algo].hash_function(input);
	len = strlen(input);
	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';                    // car echo envoit \n  mais les vrais hash avec avant de retirer dans l output.
	if (s->p && s->q)
		printf("%s\n%s\n", input, hash);
	else if (s->p)
		printf("(\"%s\")= %s\n", input, hash);
	else if (s->q)
		printf("%s\n", hash);
	else
		printf("(stdin)= %s\n", hash);
	free(input);
	free(hash);
}


static void	hash_target(t_ssl *ssl_flags, const char *arg, int is_file)
{
	char	*content;
	char	*hash;
	FILE	*f;

	if (is_file)
	{
		f = fopen(arg, "rb"); // sur windows ca buggait sinon ;/  (\r askip)
		if (!f)
		{
			fprintf(stderr, "ft_ssl: %s: %s: No such file or directory\n", g_algos[ssl_flags->algo].name, arg);
			return ;
		}
		content = read_all(f); // malloc
		fclose(f);
	}
	else
		content = strdup(arg); // dup pck si fichier on aura malloc, donc faut que le free marche dans les 2 cas
	hash = g_algos[ssl_flags->algo].hash_function(content);
	print_hash(ssl_flags, hash, (char *)arg, is_file);
	free(content);
	free(hash);
}

static int	init_algo(t_ssl *ssl_flags, const char *name)
{
	int	i;

	ssl_flags->p = false;
	ssl_flags->q = false;
	ssl_flags->r = false;
	i = 0;
	while (g_algos[i].name)
	{
		if (!strcmp(name, g_algos[i].name))
		{
			ssl_flags->algo = i;
			return (1);
		}
		i++;
	}
	fprintf(stderr, "ft_ssl: Error: '%s' is an invalid command.\n", name);
	return (0);
}

static void	print_hash(t_ssl *ssl_flags, char *hash, char *string, int is_file)
{
	char	*quotes;

	quotes = "\"";
	if (is_file)
		quotes = "";
	if (ssl_flags->q)
		printf("%s\n", hash);
	else if (ssl_flags->r)
		printf("%s  %s%s%s\n", hash, quotes, string, quotes);
	else
		printf("%s (%s%s%s) = %s\n", g_algos[ssl_flags->algo].upper, quotes, string, quotes, hash);
}

void run(int ac, char **av)
{
	t_ssl 	ssl_flags;
	int		i;
	int		has_input_or_file;
	int		file_mode;

	if (ac < 2)
	{
		printf("usage: ./ft_ssl command [flags] [file]\n");
		return ;
	}
	if (!init_algo(&ssl_flags, av[1]))
		return ;
	has_input_or_file = 0;
	for (i = 2; i < ac; i++)
	{
		if (!strcmp(av[i], "-p"))
			ssl_flags.p = true;
		else if (!strcmp(av[i], "-q"))
			ssl_flags.q = true;
		else if (!strcmp(av[i], "-r"))
			ssl_flags.r = true;
		else if (!strcmp(av[i], "-s") && ++i)
			has_input_or_file = 1;
		else
		{
			has_input_or_file = 1;
			break ;
		}
	}
	if (ssl_flags.p || !has_input_or_file)
		hash_stdin(&ssl_flags);
	file_mode = 0;
	for (i = 2; i < ac; i++)
	{
		if (!file_mode && (!strcmp(av[i], "-p") || !strcmp(av[i], "-q") || !strcmp(av[i], "-r")))
			continue ;
		if (!file_mode && !strcmp(av[i], "-s") && i + 1 < ac)
		{
			hash_target(&ssl_flags, av[++i], file_mode);
			continue ;
		}
		file_mode = 1;
		hash_target(&ssl_flags, av[i], file_mode);
	}
	return ;
}

int	main(int ac, char **av)
{
	char	*line = NULL;
	size_t	len = 0;
	char	*args[256];
	int		n;

	if (ac >= 2)
	{
		run(ac, av);
		return (0);
	}
	while (printf("ft_ssl> "), getline(&line, &len, stdin) > 0)
	{	
		line[strcspn(line, "\n")] = '\0';
		if (!*line)
			continue ;
		n = 1;
		args[0] = "ft_ssl";
		for (char *t = strtok(line, " "); t && n < 255; t = strtok(NULL, " "))  // NULL pour reprendre sur la premier string passee. 
			args[n++] = t;
		args[n] = NULL;
		run(n, args);
	}
	free(line);
	return (0);
}