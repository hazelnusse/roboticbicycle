macro(OBJCOPY_FILE EXE_NAME)
 set(FO ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.bin)
 set(FI ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME})
 message(STATUS ${FO})
 add_custom_command(
  OUTPUT "${FO}"
  COMMAND ${CMAKE_OBJCOPY}
  ARGS -O binary ${FI} ${FO}
  DEPENDS ${FI})
 get_filename_component(TGT "${EXE_NAME}" NAME)
 add_custom_target("${TGT}.bin" ALL DEPENDS ${FO} VERBATIM)
 get_directory_property(extra_clean_files ADDITIONAL_MAKE_CLEAN_FILES)
 set_directory_properties(
  PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "${extra_clean_files};${FO}")
 set_source_files_properties("${FO}" PROPERTIES GENERATED TRUE)
endmacro(OBJCOPY_FILE)

macro(OBJDUMP_FILE EXE_NAME)
 set(FO ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.list)
 set(FI ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME})
 message(STATUS ${FO})
 add_custom_command(
  OUTPUT "${FO}"
  COMMAND ${CMAKE_OBJDUMP}
  ARGS -S ${FI} > ${FO}
  DEPENDS ${FI})
 get_filename_component(TGT "${EXE_NAME}" NAME)
 add_custom_target("${TGT}.list" ALL DEPENDS ${FO} VERBATIM)
 get_directory_property(extra_clean_files ADDITIONAL_MAKE_CLEAN_FILES)
 set_directory_properties(
  PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "${extra_clean_files};${FO}")
 set_source_files_properties("${FO}" PROPERTIES GENERATED TRUE)
endmacro(OBJDUMP_FILE)
