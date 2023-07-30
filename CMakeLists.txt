cmake_minimum_required(VERSION 3.0)
project(cppposit)

#set(SYSPROC arm)
if(SYSPROC MATCHES "arm")
add_definitions (-D__arm__ )
ENDIF()

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" /WX "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-long-long   -pedantic")
endif()

add_definitions(--std=c++2b)


include_directories(include)
find_package(Eigen3 )
include_directories(${EIGEN3_INCLUDE_DIRS})

include_directories(extern)

add_subdirectory(examples)


