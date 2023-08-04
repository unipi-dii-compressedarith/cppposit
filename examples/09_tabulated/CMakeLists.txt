add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h
  COMMAND python ${PROJECT_SOURCE_DIR}/scripts/generatePositCppTables.py 
    -n 4 -e 0
    -o ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h
  DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generatePositCppTables.py
)

add_executable(example_09_tabulated ${CMAKE_CURRENT_SOURCE_DIR}/09_tabulated.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ptab4_0.h)
