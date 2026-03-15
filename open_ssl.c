


si on a ac == 1


void split_flat_parse()
{

	set_flags(input flags, nom du fichier)

}


void ft_open_ssl(char *input)
{
	char **split = ft_split(input);

	if (split[0] == "md5")
	{
		split_flat_parse(split);
	}
	else if (split[0] == "sha256")
	{
		split_flat_parse(split);

	}
	else if (split[0] == "whirlpool")
	{
		split_flat_parse(split);
	}
	else
	{
		printf("Error, bad input");
		return;
	}

	return;
}