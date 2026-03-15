


typedef struct s_md5
{
	bool p_flag;
	bool _flag;
	bool _flag;
	bool _flag;
} t_md5;



typedef struct s_sha256
{
    bool p_flag;
    bool _flag;
    bool _flag;
    bool _flag;
} t_sha256;



void	free_tab(char **tab, size_t n);
char	**ft_split(const char *s, char c);