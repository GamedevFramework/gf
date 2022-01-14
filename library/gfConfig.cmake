if (NOT gf_FIND_COMPONENTS)
  set (gf_FIND_COMPONENTS "graphics")
endif()

include (CMakeFindDependencyMacro)

set (GF_COMPONENTS "${gf_FIND_COMPONENTS}")
if (gf_FIND_REQUIRED)
  set (ERROR_LEVEL "FATAL_ERROR")
else ()
  set (ERROR_LEVEL "STATUS")
endif ()

# Include all gf targets
get_filename_component (GF_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
include ("${GF_CMAKE_DIR}/gfTargets.cmake")

# Update components list depending on internal dependencies
foreach (component ${gf_FIND_COMPONENTS})
  if ((${component} STREQUAL "graphics") OR (${component} STREQUAL "net"))
    list (APPEND GF_COMPONENTS "core")
  endif ()
endforeach ()
list(REMOVE_DUPLICATES GF_COMPONENTS)

# Macro to load dependencies
macro (load_dependencies component)
  # For gf::core
  if (${component} STREQUAL "core")
    find_dependency(Threads)
    find_dependency(Boost REQUIRED)
    find_dependency(ZLIB)
    if (NOT BUILD_SHARED_LIBS)
      find_dependency(pugixml)
    endif ()

  # For gf::graphics
  elseif (${component} STREQUAL "graphics")
    find_dependency(Threads)
    if (NOT BUILD_SHARED_LIBS)
      find_dependency(SDL2)
      find_dependency(Freetype)
      find_dependency(pugixml)
    endif()

  # For gf::net
  elseif (${component} STREQUAL "net")
    # No dependencies

  else ()
    message(${ERROR_LEVEL} "'${component}' is unknown")
  endif ()
endmacro ()


# Add all component
set (GF_LIBRARIES "")
foreach (component ${GF_COMPONENTS})
  if (TARGET "gf::${component}")
    set(GF_FOUND TRUE)

    # Load dependencies
    load_dependencies(${component})

    # Add component
    list (APPEND GF_LIBRARIES "gf::${component}")
  else ()
    message(${ERROR_LEVEL} "'gf::${component}' component not found")
  endif ()
endforeach ()

# Get GF version info
include ("${GF_CMAKE_DIR}/gfConfigVersion.cmake")

if ((NOT gf_FIND_REQUIRED) AND (NOT GF_FOUND))
  message (${ERROR_LEVEL} "GF not found: skipped")
elseif ((gf_FIND_REQUIRED) AND (NOT GF_FOUND))
  message (${ERROR_LEVEL} "GF not found: error")
else ()
  message(STATUS "Found GF (version \"${PACKAGE_VERSION}\") in ${CMAKE_CURRENT_LIST_DIR} with components: ${GF_COMPONENTS}")
endif ()
