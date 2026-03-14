NAME		= ft_ssl

CC		= cc
CFLAGS		= -Wall -Wextra -Werror

SRCS		= # tes sources ici
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ------------------------------------------------
#  Docker - reference OpenSSL 1.1.1
# ------------------------------------------------

DOCKER_IMAGE	= ft_ssl_ref

docker:
	@if docker image inspect $(DOCKER_IMAGE) > /dev/null 2>&1; then \
		echo "\033[33m[Docker] Image '$(DOCKER_IMAGE)' already built.\033[0m"; \
	else \
		echo "\033[33m[Docker] Building image '$(DOCKER_IMAGE)'...\033[0m"; \
		docker build -t $(DOCKER_IMAGE) .; \
		echo "\033[32m[Docker] Build done.\033[0m"; \
	fi
	@echo ""
	@echo "\033[36m--- Pour entrer dans le container ---\033[0m"
	@echo "  docker run -it $(DOCKER_IMAGE)"
	@echo ""
	@echo "\033[36m--- Une fois dans le container, lancer OpenSSL ---\033[0m"
	@echo "  openssl"
	@echo ""
	@echo "\033[36m--- Puis taper ces commandes dans le prompt OpenSSL> ---\033[0m"
	@echo "  md5 /tmp/file"
	@echo "  md5 -r /tmp/file"
	@echo "  sha256 /tmp/file"
	@echo "  sha256 -r /tmp/file"

docker-clean:
	docker rmi -f $(DOCKER_IMAGE)

.PHONY: all clean fclean re docker docker-clean