CC = cc
CFLAGS = -Wall -Wextra -Werror
CFILES = main.c ft_atoi.c
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