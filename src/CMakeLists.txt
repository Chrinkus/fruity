add_library(fruity
	"fruity.c"
        "fruity_io.c"
        "fruity_point.c"
        "fruity_set.c"
)
target_include_directories(fruity PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(fruity cgs)
