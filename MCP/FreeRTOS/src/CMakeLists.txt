include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/inc)

add_library(FreeRTOS
        heap_1.c
        list.c
        port.c
        queue.c
        tasks.c
        timers.c)
