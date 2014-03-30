#  Dependencies:
#  	- libpng
#

CXX				?= clang++
CXXFLAGS 	+= -g -std=c++11
LDFLAGS		+= -lpng

EXE  = SimpleSpritePacker.exe
SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SRCS))

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) $(OBJS) $(EXE)
