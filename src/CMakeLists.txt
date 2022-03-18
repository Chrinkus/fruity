set(sources
	"fruity.c"
	"fruity_io.c"
)

set(headers
	"fruity.h"
	"fruity_io.h"
)

add_library(${LIB_NAME} "${sources}")

set_target_properties(${LIB_NAME}
	PROPERTIES
		VERSION		${PROJECT_VERSION}
		SOVERSION	${PROJECT_VERSION_MAJOR}
		PUBLIC_HEADER	"${headers}"
)

install(TARGETS ${LIB_NAME}
	EXPORT ${LIB_NAME}
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

