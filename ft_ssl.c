


 open
◦ close
◦ read
◦ write
◦ malloc
◦ free


int main(int ac, char **av)
{
    if (ac < 3 || ac > 7)  // flag ou pas flag
    {
        printf("Error: wrong number of arguments");
        return (-1)
    }
    if (argv[1] == "md5")
    {
        ft_md5();
    }
    else if(argv[1] == "sha256")
    {

    }
    else if(argv[1] == "whirlpool")
    {
        ft_sha256();
    }
    else
    {
        
    }
    return (0);
}

char *ft_sll()
{


}