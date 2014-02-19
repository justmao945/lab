LIBPEL := lib$(PEL).a

INC_DIRS  = -I../include/
CXX_FILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CXX_FILES))

all: $(OBJS)
	$(AR) rcs $(LIBPEL) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIBPEL)

.PHONY: clean
