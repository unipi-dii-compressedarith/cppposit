add_custom_target(examples)


MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
FOREACH(subdir ${SUBDIRS})
  if(subdir STREQUAL "09_tabulated")
    continue() 
  endif(subdir STREQUAL "09_tabulated")
  
  add_executable(example_${subdir} ${subdir}/${subdir}.cpp)
  target_compile_definitions(example_${subdir} PRIVATE POSIT_VALUE_LOGGER POSIT) 
  list(APPEND example_targets example_${subdir})
ENDFOREACH()
add_dependencies(examples ${example_targets})


add_subdirectory(09_tabulated)
