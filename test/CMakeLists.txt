include_directories("${PROJECT_SOURCE_DIR}/src")

find_package(cmocka REQUIRED)

add_custom_target(tests_fruity)

set(test_sources
	"tests_fruity.c"
        "tests_fruity_structs.c"
        #"tests_fruity_io.c"
)

string(LENGTH "tests_" test_prefix_len)

foreach(file IN LISTS test_sources)
	get_filename_component(file_we "${file}" NAME_WE)
	string(SUBSTRING "${file_we}" ${test_prefix_len} -1 func)
	set(bin_name "${func}_test")

	add_executable("${bin_name}" EXCLUDE_FROM_ALL "${file}")
	target_link_libraries("${bin_name}" PRIVATE "${LIB_NAME}" cmocka)
	add_test(NAME "${func}" COMMAND "${bin_name}")
	add_dependencies(tests_fruity "${bin_name}")
endforeach()

