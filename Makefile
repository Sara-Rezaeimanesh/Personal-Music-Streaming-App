CC=g++ -std=c++11

all: a.out

a.out: main.o exceptions.o utunes.o music.o user.o commonfuncs.o playlist.o comment.o handlers.o utilities.o request.o response.o template_parser.o filter.o server.o route.o
	$(CC) main.o exceptions.o utunes.o music.o user.o commonfuncs.o playlist.o comment.o handlers.o utilities.o request.o response.o template_parser.o filter.o server.o route.o -o utunes.out

main.o: main.cpp main.hpp utunes.hpp
	$(CC) -c main.cpp main.hpp utunes.hpp

exceptions.o: exceptions.cpp exceptions.hpp
	$(CC) -c exceptions.cpp exceptions.hpp

user.o: user.cpp user.hpp playlist.hpp
	$(CC) -c user.cpp user.hpp playlist.hpp

utunes.o: utunes.cpp utunes.hpp music.hpp user.hpp commonfuncs.hpp server/server.hpp
	$(CC) -c utunes.cpp utunes.hpp music.hpp user.hpp commonfuncs.hpp server/server.hpp

music.o: music.cpp music.hpp comment.hpp
	$(CC) -c music.cpp music.hpp comment.hpp

playlist.o: playlist.cpp playlist.hpp commonfuncs.hpp
	$(CC) -c playlist.cpp playlist.hpp commonfuncs.hpp

commonfuncs.o: commonfuncs.cpp commonfuncs.hpp
	$(CC) -c commonfuncs.cpp commonfuncs.hpp

comment.o: comment.cpp comment.hpp
	$(CC) -c comment.cpp comment.hpp

filter.o: filter.cpp filter.hpp playlist.hpp
	$(CC) -c filter.cpp filter.hpp playlist.hpp

handlers.o: handlers.cpp handlers.hpp server/server.hpp
	$(CC) -c handlers.cpp handlers.hpp server/server.hpp

template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) -c utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp

response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) -c utils/response.cpp utils/response.hpp utils/include.hpp

request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) -c utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp

utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) -c utils/utilities.cpp utils/utilities.hpp

server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) -c server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp 

route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) -c server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp 

.PHONY: clean
clean:
	rm *.o
	rm utunes.out
