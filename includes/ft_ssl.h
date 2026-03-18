#ifndef FT_SSL_H
# define FT_SSL_H

# include "md5.h"
# include "sha256.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

typedef char	*(*t_hash)(const char *); // return, type de la ft, param

typedef struct s_algo
{
	char	*name;
	char	*upper;
	t_hash	hash_function;
}	t_algo;

typedef struct s_ssl
{
	int		algo;
	bool	p;
	bool	q;
	bool	r;
}	t_ssl;

static void	print_hash(t_ssl *s, char *hash, char *label, int is_file);


#endif