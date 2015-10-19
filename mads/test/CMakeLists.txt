add_executable(taobject taobject.cpp)
target_link_libraries(taobject mipslib)
  
add_executable(tasm tasm.cpp)
target_link_libraries(tasm mipslib)

add_executable(tdisasm tdisasm.cpp)
target_link_libraries(tdisasm mipslib)

add_executable(tprepare tprepare.cpp)
target_link_libraries(tprepare mipslib)

add_executable(tsim tsim.cpp)
target_link_libraries(tsim mipslib)

add_executable(tutils tutils.cpp)
target_link_libraries(tutils mipslib)
