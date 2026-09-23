#   include(cmake/GitPatch.cmake)
#   git_apply_patch(third_party/libfoo patches/0001-fix.patch patches/0002-more.patch)

find_package(Git REQUIRED)

function(git_apply_patch work_dir)
  get_filename_component(work_dir "${work_dir}" ABSOLUTE)

  foreach(p IN LISTS ARGN)
    get_filename_component(patch "${p}" ABSOLUTE)

    # Skip if the patch is already applied (it can be cleanly reversed).
    execute_process(
      COMMAND ${GIT_EXECUTABLE} apply --reverse --check "${patch}"
      WORKING_DIRECTORY "${work_dir}"
      RESULT_VARIABLE already_applied OUTPUT_QUIET ERROR_QUIET)
    if(already_applied EQUAL 0)
      continue()
    endif()

    execute_process(
      COMMAND ${GIT_EXECUTABLE} apply "${patch}"
      WORKING_DIRECTORY "${work_dir}"
      RESULT_VARIABLE result ERROR_VARIABLE error)
    if(NOT result EQUAL 0)
      message(FATAL_ERROR "Failed to apply ${patch}:\n${error}")
    endif()
    message(STATUS "Applied patch: ${p}")
  endforeach()
endfunction()