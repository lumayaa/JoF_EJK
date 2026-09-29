cmake_minimum_required(VERSION 3.16)
project(target_media_check C)
set(repo "${CMAKE_CURRENT_SOURCE_DIR}/../..")
configure_file("${repo}/codemp/versionfile/jofversion.h.in" jofversion.h @ONLY)
add_executable(target_media_check check.c
  "${repo}/codemp/game/g_media.c"
  "${repo}/codemp/qcommon/q_shared.c"
  "${repo}/shared/qcommon/q_color.c"
  "${repo}/shared/qcommon/q_math.c"
  "${repo}/shared/qcommon/q_string.c")
target_include_directories(target_media_check PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}" "${repo}/codemp" "${repo}/shared")
if(MSVC)
  target_compile_definitions(target_media_check PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_link_libraries(target_media_check PRIVATE m)
endif()
enable_testing()
foreach(case encounter ownership recipients events cleanup validation)
  add_test(NAME media_${case} COMMAND target_media_check ${case})
endforeach()
