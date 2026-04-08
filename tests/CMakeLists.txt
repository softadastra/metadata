add_executable(softadastra_metadata_test_basic
    metadata_test_basic.cpp
)

target_link_libraries(softadastra_metadata_test_basic
    PRIVATE softadastra::metadata
)

add_test(NAME softadastra_metadata_test_basic
         COMMAND softadastra_metadata_test_basic)
