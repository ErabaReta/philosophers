CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread #-g3 -fsanitize=thread
CFILES = main.c ft_philo_atoi.c exiter.c watcher.c logged_checkers.c philo.c init.c eating.c parsing.c  lonely_philo.c wait_for.c finish.c
OFILES = $(CFILES:.c=.o)
RM = rm -rf
NAME = philo

all: $(NAME)

$(NAME) : $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -lpthread -o $(NAME)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OFILES)

fclean : clean
	$(RM) $(NAME)

re : fclean all
