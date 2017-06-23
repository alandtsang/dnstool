SRC		:= src
SOURCE  := $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET  := dnstool

CC      := g++
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I./include
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H -std=c++11


.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean everything

clean :
	@rm -fr $(SRC)/*.o $(TARGET)

veryclean : clean
	rm -fr $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
