NAME = philo
CC = cc
CFLAGS = -g -lpthread -Werror -Wextra -Wall
SRCS = main.c philosim.c routines.c big_brother.c utilities.c

all: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:

fclean: $(NAME)
	rm $(NAME)
re: fclean all

.PHONY: all clean fclean re
