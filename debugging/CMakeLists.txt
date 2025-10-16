set(DEBUGGER_NEEDED_BINS openocd)

set(DEBUGGER_FOUND TRUE)

foreach(DEBUGGER_NEEDED_BIN ${DEBUGGER_NEEDED_BINS})
  find_program(${DEBUGGER_NEEDED_BIN}_BIN_PATH ${DEBUGGER_NEEDED_BIN} NO_CACHE)
  if(NOT ${DEBUGGER_NEEDED_BIN}_BIN_PATH)
    set(DEBUGGER_FOUND FALSE)
  endif()
endforeach()

if(NOT DEBUGGER_FOUND)
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    execute_process(
      COMMAND uname -m
      OUTPUT_VARIABLE HOST_ARCH
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(HOST_ARCH STREQUAL "x86_64")
      set(DEBUGGER_URL
          "https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-linux-x64.tar.gz"
      )
      set(DEBUGGER_FOLDER "openocd")
    elseif(HOST_ARCH STREQUAL "aarch64")
      set(DEBUGGER_URL
          "https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-linux-arm64.tar.gz"
      )
      set(DEBUGGER_FOLDER "openocd")
    endif()
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(HOST_ARCH $ENV{PROCESSOR_ARCHITECTURE})
    if(HOST_ARCH STREQUAL "AMD64")
      set(HOST_ARCH "x86_64")
    endif()
    if(HOST_ARCH STREQUAL "x86_64")
      set(DEBUGGER_URL
          "https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-win32-x64.zip"
      )
      set(DEBUGGER_FOLDER "openocd")
    elseif(HOST_ARCH STREQUAL "aarch64")
      message(FATAL_ERROR "Get openocd yourself!")
    endif()

  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    execute_process(
      COMMAND uname -m
      OUTPUT_VARIABLE HOST_ARCH
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(HOST_ARCH STREQUAL "x86_64")
      set(DEBUGGER_URL
          "https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-darwin-x64.tar.gz"
      )
      set(DEBUGGER_FOLDER
          "openocd")
    elseif(HOST_ARCH STREQUAL "arm64")
      set(DEBUGGER_URL
          "https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-darwin-arm64.tar.gz"
      )
      set(DEBUGGER_FOLDER
          "openocd")
    endif()
  endif()

  # Download and extract toolchain if not present
  set(DEBUGGER_DIR "${CMAKE_BINARY_DIR}/${DEBUGGER_FOLDER}")
  if(NOT EXISTS "${DEBUGGER_DIR}")
    message(
      STATUS
        "Downloading openOCD for ${CMAKE_HOST_SYSTEM_NAME} ${HOST_ARCH}..."
    )
    get_filename_component(ARCHIVE_NAME "${DEBUGGER_URL}" NAME)
    file(DOWNLOAD "${DEBUGGER_URL}" "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}"
         SHOW_PROGRESS)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf "${ARCHIVE_NAME}"
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    file(REMOVE "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}")
  endif()
endif()
