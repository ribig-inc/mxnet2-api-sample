CC  = g++
TARGET  = apitest
SRCS    = test.cpp
OBJS    = $(SRCS:.cpp=.o)

LIBS    = -l dl -l rt -l mxnet2api -l pthread

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

all: clean $(OBJS) $(TARGET)
clean:
	-rm -f $(OBJS) $(TARGET) 

