set(TEST_TARGET_NAME testToDo)

set(${TEST_TARGET_NAME}_SRC
        testToDo.cpp
)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})

target_link_libraries(${TEST_TARGET_NAME} BayesFilters)

add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
