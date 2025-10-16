if(NOT EXISTS "${CMAKE_BINARY_DIR}/uniflash_linux")
  file(
    DOWNLOAD
    "https://github.com/jasonpcarroll/supported-target-tirm57l/releases/download/lol/uniflash_linux.zip"
    "${CMAKE_BINARY_DIR}/uniflash_linux.zip"
    SHOW_PROGRESS)

  file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/uniflash_linux.zip" DESTINATION "${CMAKE_BINARY_DIR}/uniflash_linux")

  file(REMOVE "${CMAKE_BINARY_DIR}/uniflash_linux.zip")
endif()
