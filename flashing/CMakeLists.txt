if(NOT EXISTS "${CMAKE_BINARY_DIR}/uniflash_linux")
  file(
    DOWNLOAD
    "https://github.com/jasonpcarroll/supported-target-tirm57l/releases/download/lol/uniflash_linux.zip"
    "${CMAKE_BINARY_DIR}/uniflash_linux.zip"
    SHOW_PROGRESS)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf
            "${CMAKE_CURRENT_LIST_DIR}/uniflash_linux.zip"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

  file(REMOVE "${CMAKE_BINARY_DIR}/uniflash_linux.zip")
endif()
