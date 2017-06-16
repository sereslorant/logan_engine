
set(TEST_JSON_SRC TestJSON.cpp)

add_executable(TestJSON ${TEST_JSON_SRC})
target_link_libraries(TestJSON lJSON)
