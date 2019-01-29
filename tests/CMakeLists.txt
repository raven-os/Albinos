###### CTest ######
include(CTest)
###################

###### GTest ######
find_package(GTest REQUIRED)

set(SOURCES
        lib-test.cpp
        blob_test.cpp
        config_db_test.cpp)
CREATE_UNIT_TEST(lib-test libconfig "${SOURCES}")
target_link_libraries(lib-test uv_a ${filesystem_lib} ${sqlite3_lib})
target_include_directories(lib-test PUBLIC ../vendor/uvw/src ../vendor/uvw/deps/libuv/include ../vendor/json ../vendor/sql/hdr ../service)