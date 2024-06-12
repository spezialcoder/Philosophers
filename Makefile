NAME = philo
CC = cc
CFLAGS = -g -lpthread
SRCS = main.c

all: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:

fclean:

re:

.PHONY: all clean fclean re