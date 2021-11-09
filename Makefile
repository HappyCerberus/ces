CXX = g++
WARNINGS = -pedantic -Wall -Wextra -Werror
WDISABLE = 
DEBUGFLAGS = -ggdb3 -O0
RELEASEFLAGS = -O3
CXXFLAGS = -std=c++20 -fcoroutines $(DEBUGFLAGS) $(WARNINGS) $(WDISABLE)
LDFLAGS = 

SRCS = utils/handle-error.cc
OBJS = $(SRCS:.cc=.o)
HTTP_CLIENT_DEMO: http-client-demo.cc $(SRCS)

all: $(HTTP_CLIENT_DEMO)

$(MAIN): $(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LDFLAGS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)
