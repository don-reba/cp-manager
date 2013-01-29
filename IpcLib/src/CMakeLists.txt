cmake_minimum_required(VERSION 2.6)
project(ipclib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

add_library(ipclib SHARED
		Protocol.cpp
		SocketServer.cpp
		SocketClient.cpp
		SystemException.cpp
		)

file(COPY
	IProtocol.h
	ITransport.h
	Protocol.h
	SocketClient.h
	SocketServer.h
	SystemException.h
	DESTINATION "${PROJECT_SOURCE_DIR}/../include"
	)

set_target_properties(ipclib
		PROPERTIES COMPILE_FLAGS "-ffor-scope -Werror -Wall -O3 -g"
		)

find_package(Doxygen)

if (Doxygen_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
			)
endif(Doxygen_FOUND)
