add_library(fruity
	"fruity.c"
        "fruity_io.c"
)
target_include_directories(fruity PUBLIC "${PROJECT_SOURCE_DIR}/include")
