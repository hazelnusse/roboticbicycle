include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR}/inc)
set(scp_sources main.cc mainwindow.cc)
set(scp_headers ${PROJECT_SOURCE_DIR}/inc/mainwindow.h)

qt4_wrap_cpp(scp_headers_moc ${scp_headers})
add_executable(scp ${scp_sources} ${scp_headers_moc})
target_link_libraries(scp ${QT_LIBRARIES})

# Test code
add_subdirectory(test)
