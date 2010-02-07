# HTTP frontend
MESSAGE( STATUS "HTTP frontend check" )

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     ${ASIO_INCLUDE_DIR}
			)

set(RPHPWEB_SRC_FILES
  connection.cpp
  mime_types.cpp
  posix_main.cpp
  reply.cpp
  request_handler.cpp
  request_parser.cpp
  server.cpp
#  win_main.cpp
)

add_executable( rphp-web ${RPHPWEB_SRC_FILES} )

target_link_libraries( rphp-web
                       pthread
                     )

