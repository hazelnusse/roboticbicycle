include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR}/../MCP/inc
                    ${PROJECT_SOURCE_DIR}/inc
                    ${VTK_INCLUDE_DIRS})

set(scp_sources main.cc
                mainwindow.cc
                plotwidget.cc
                ${PROJECT_SOURCE_DIR}/../MCP/src/sample.cc)

set(scp_headers ${PROJECT_SOURCE_DIR}/inc/mainwindow.h
                ${PROJECT_SOURCE_DIR}/inc/plotwidget.h)

qt4_wrap_cpp(scp_headers_moc ${scp_headers})
add_executable(scp ${scp_sources} ${scp_headers_moc})
target_link_libraries(scp ${QT_LIBRARIES}
                          QVTK
                          vtkRendering
                          vtkCharts)

# Test code
add_subdirectory(test)
