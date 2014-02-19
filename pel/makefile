# Default complition and link requirements:
# 1. clang
# 2. tcmalloc (x?x)
# 3. pthread (for test)

PEL    = pel
TEST   = test
LIBPEL:= lib$(PEL)

AR 	= ar
CXX = clang++
CXXFLAGS ?= -g -O0

# Option -static causes valgrind error 'Conditional jump or move depends on
# uninitialised value(s)' of C library functions.
LDFLAGS := -l$(PEL) -lm # -ltcmalloc_minimal
INC_DIRS = -I./include/
LIB_DIRS = -L./lib/

CXX_FILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CXX_FILES))

all: $(PEL) $(TEST)

$(PEL): $(LIBPEL) $(OBJS)
	$(CXX) $(LIB_DIRS) -o $(PEL) $(PEL).o $(LDFLAGS)

$(TEST): $(LIBPEL) $(OBJS)
	$(CXX) $(LIB_DIRS) -o $(TEST) $(TEST).o $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

libpel: $(PEL_FILES)
	AR="$(AR)" CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" PEL="$(PEL)" $(MAKE) -C lib/

clean:
	$(RM) $(OBJS) $(PEL) $(TEST)
	PEL="$(PEL)" $(MAKE) -C lib/ clean

.PHONY: all libpel clean
