# include <stdlib.h>
# include <string.h> 

int	count_words(const char *s, char c)
{
	size_t	n;

	n = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			n++;
		while (*s && *s != c)
			s++;
	}
	return (n);
}

char	*extract(const char *s, char c)
{
	const char	*end;
	char		*word;
	size_t		len;

	end = s;
	while (*end && *end != c)
		end++;
	len = (size_t)(end - s);
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, s, len);
	word[len] = '\0';
	return (word);
}

void	free_tab(char **tab, size_t n)
{
	while (n--)
		free(tab[n]);
	free(tab);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	i;

	if (!s)
		return (NULL);
	tab = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			tab[i] = extract(s, c);
			if (!tab[i])
				return (free_tab(tab, i), NULL);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	tab[i] = NULL;
	return (tab);
}
