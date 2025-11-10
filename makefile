
# in linux:
# CPPFLAGS = -std=c++11 -Wall -g -fsanitizer=-fsanitize=address,leak,undefined

CPPFLAGS = -std=c++11 -Wall
SRCS = route.cpp main.cpp characters.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
EXE = mini_star_rail

$(EXE) : $(OBJS)
	g++ $(CPPFLAGS) -o $@ $^

-include $(DEPS)

%.o: %.cpp
	g++ $(CPPFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm $(EXE) $(OBJS) $(DEPS)