project(mads)
set(CMAKE_BUILD_TYPE Release)
set(SRC src)
set(TEST test)

include_directories(${SRC})

set(MADS_SRCS
  ${SRC}/adef.h
  ${SRC}/except.h
  ${SRC}/storage.h
  ${SRC}/logger.h
  ${SRC}/utils.h   ${SRC}/utils.cpp

  ${SRC}/aobject.h ${SRC}/aobject.cpp
  ${SRC}/asm.h     ${SRC}/asm.cpp
  ${SRC}/disasm.h  ${SRC}/disasm.cpp
  ${SRC}/prepare.h ${SRC}/prepare.cpp
  ${SRC}/sim.h     ${SRC}/sim.cpp)

add_library(mipslib STATIC ${MADS_SRCS})

add_subdirectory(${SRC} bin)
add_subdirectory(${TEST} bin)
