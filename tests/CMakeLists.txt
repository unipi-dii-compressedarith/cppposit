include(FetchContent)
# declare Catch2
FetchContent_Declare(Catch2                              # name of the content
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git  # the repository
  GIT_TAG        v2.13.7                                 # the tag
  )

# make available
FetchContent_MakeAvailable(Catch2)

add_custom_target(tests)

add_executable(test_init test_init.cpp)
target_link_libraries(test_init PRIVATE Catch2::Catch2)

add_executable(test_subnormals test_subnormals.cpp)
target_link_libraries(test_subnormals PRIVATE Catch2::Catch2)

add_executable(test_coverage test_coverage.cpp)
target_compile_options(test_coverage PUBLIC  -fprofile-arcs -ftest-coverage -O0)
target_link_libraries(test_coverage PRIVATE Catch2::Catch2 gcov)


add_dependencies(tests test_init test_subnormals test_coverage)

add_test(
    NAME test_init
    COMMAND test_init --success
)

add_test(
    NAME test_subnormals
    COMMAND test_subnormals --success
)

add_test (
   NAME test_coverage
   COMMAND test_subnormals --success
)