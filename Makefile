	
SOURCE = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))
	
CXX = gcc

CFLAGS += -std=c++11
LDFLAGS += -lm -lpthread
	
all:bts
bts:$(OBJS)
	$(CXX) -Wall $(CFLAGS) ${LDFLAGS} -o $@ $^

.PHONY:clean
clean:
	rm -f *.o *.d bts

include $(SOURCE:.cpp=.d)	
	
%.o:%.cpp
	$(CXX) -Wall $(CFLAGS) -c $< -o $@

