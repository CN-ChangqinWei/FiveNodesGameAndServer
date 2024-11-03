SRC=$(wildcard *.cpp)
DIF=$(patsubst %.cpp,%.o,$(SRC))
TARGET=Server.Out
INC=../inc
LIB=-lpthread -lmysqlclient
LL=/usr/include/mysql
CC=g++
CFLAG=-std=c++11 -g -Wall $(LIB)

$(TARGET):$(DIF)
	$(CC) $^ $(CFLAG) -o $@
%.o:%.cpp
	$(CC) $^ -I$(INC) -L$(LL) $(CFLAG) -c
clean:
	rm *.o $(TARGET) -f	

