set(TEST_TARGET_NAME test_mixed_UKF_KF)

set(${TEST_TARGET_NAME}_SRC
        main.cpp
)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})

target_link_libraries(${TEST_TARGET_NAME} BayesFilters)

add_test(NAME ${TEST_TARGET_NAME}
         COMMAND ${TEST_TARGET_NAME}
         WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)
