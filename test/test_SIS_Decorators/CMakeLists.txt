set(TEST_TARGET_NAME test_SIS_Decorators)

set(${TEST_TARGET_NAME}_SRC
        main.cpp
)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})

target_link_libraries(${TEST_TARGET_NAME} BayesFilters)

if(UNIX AND NOT APPLE)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

	target_link_libraries(${TEST_TARGET_NAME} Threads::Threads)
endif()

add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
