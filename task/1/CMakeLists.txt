include(config.cmake)
message("antlr4-runtime_DIR: ${antlr4-runtime_DIR}")
if(TASK1_WITH STREQUAL "flex")
  message(AUTHOR_WARNING "使用 Flex 完成实验一")
  add_subdirectory(flex)

elseif(TASK1_WITH STREQUAL "antlr")
  message(AUTHOR_WARNING "使用 ANTLR 完成实验一")
  add_subdirectory(antlr)

else()
  message(FATAL_ERROR "无效的 TASK1_WITH 取值：${TASK1_WITH}")

endif()
