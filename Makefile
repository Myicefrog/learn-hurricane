SOURCE = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))
	
CXX = g++
INCLUDE = -I.

CFLAGS += -std=c++11
LDFLAGS += -lm -lpthread

TARGET = learn
	
ALL:${OBJS}
	${CXX} -o  ${TARGET} ${OBJS} ${LDFLAGS}

.PHONY:clean
clean:
	rm -f *.o learn

%.o:%.cpp
	${CXX} -Wall ${CFLAGS} ${INCLUDE} -c $< -o $@

