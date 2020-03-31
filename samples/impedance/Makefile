CXX = g++
CXXFLAGS += -Wall
LDLIBS = -lyal -lyalrt -lpthread

SRCS = $(wildcard ./*.cpp)
OBJS = $(SRCS:.cpp=.o)

# 複数ファイルを別々の実行ファイルにしたい時
TARGET = $(basename $(SRCS))
# PROGRAMS = main #rttask

# 複数ファイルをまとめて1つの実行ファイルにしたい時
# TARGET := target

all : $(TARGET) 

# $(TARGET) : $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

# %.o : %.cpp
# 	$(CXX) -c $(CXXFLAGS) -o $@ $< $(LDLIBS)

clean:
	$(RM) *.o *.out $(PROGRAMS) $(TARGET)

# SRCS :=
# include $(fifo) Makefile
# include $(upd) Makefile
# include $(filter) Makefile
