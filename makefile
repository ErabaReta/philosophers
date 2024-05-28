CC = cc
CFLAGS = -Wall -Wextra -Werror #-g #-fsanitize=thread
CFILES = main.c ft_atoi.c exiter.c watcher.c  ft_itoa.c printer.c #noter.c
OFILES = $(CFILES:.c=.o)
RM = rm -rf
NAME = philo

all: $(NAME)

$(NAME) : $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o $(NAME)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OFILES)

fclean : clean
	$(RM) $(NAME)

re : fclean all