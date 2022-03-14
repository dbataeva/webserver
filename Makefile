NAME	=	webserv

SRCS	=	srcs/main_macos.cpp srcs/save_types.cpp \
			srcs/parser_env.cpp srcs/response.cpp \
			srcs/save_responses.cpp srcs/Pars_conf.cpp \
			srcs/Location.cpp srcs/Autoindex.cpp srcs/error.cpp

OBJS	=	$(SRCS:.cpp=.o)

CPFLAGS	=	-Wall -Werror -Wextra -g -std=c++98

RM		=	rm -f

.cpp.o:		includes/parser_env.hpp includes/response.hpp \
			includes/Pars_conf.hpp includes/Location.hpp \
			includes/Autoindex.hpp includes/error.hpp \
			includes/main_macos.hpp
			clang++ $(CPFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			c++ $(CPFLAGS) $(OBJS) -o $(NAME)
			mkdir cgi-bin/file/downloads
			mkdir cgi-bin/registr/users

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)
			rm -rf cgi-bin/file/downloads
			rm -rf cgi-bin/registr/users

re:			fclean $(NAME)
			
.PHONY:		all clean fclean re