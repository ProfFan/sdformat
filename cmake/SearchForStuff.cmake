include (${sdf_cmake_dir}/SDFUtils.cmake)
include (${sdf_cmake_dir}/FindOS.cmake)
include (FindPkgConfig)

########################################
# Find Boost, if not specified manually
include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem program_options regex iostreams)

if (NOT Boost_FOUND)
  set (BUILD_SDF OFF CACHE INTERNAL "Build SDF" FORCE)
  BUILD_ERROR ("Boost not found. Please install thread signals system filesystem program_options regex boost version ${MIN_BOOST_VERSION} or higher.")
endif() 

#################################################
# Find tinyxml. Only debian distributions package tinyxml with a pkg-config
# Use pkg_check_modules and fallback to manual detection (needed, at least, for MacOS)
pkg_check_modules(tinyxml tinyxml)
if (NOT tinyxml_FOUND)
  find_path (tinyxml_include_dirs tinyxml.h ${tinyxml_include_dirs} ENV CPATH)
  find_library(tinyxml_LIBRARIES NAMES tinyxml)
  set (tinyxml_FAIL False) 
  if (NOT tinyxml_include_dirs)
    message (STATUS "Looking for tinyxml headers - not found")
    set (tinyxml_FAIL True) 
  endif()
  if (NOT tinyxml_LIBRARIES)
    message (STATUS "Looking for tinyxml library - not found")
    set (tinyxml_FAIL True) 
  endif()
endif()
    
if (tinyxml_FAIL)
  message (STATUS "Looking for tinyxml.h - not found")
  BUILD_ERROR("Missing: tinyxml")
endif()

################################################
# Find urdfdom parser
if (USE_EXTERNAL_URDF)
  if (NOT PKG_CONFIG_FOUND)
      BUILD_ERROR ("pkgconfig not found. Please install to so USE_EXTERNAL_URDF can found the urdf library")
  else()
    # check for urdfdom with pkg-config
    pkg_check_modules(URDF urdfdom>=0.3)

    if (NOT URDF_FOUND)
      # version >= 0.3.x not found, check for urdfdom again with no version
      # restriction.
      pkg_check_modules(URDF urdfdom)


      if (NOT URDF_FOUND)
        BUILD_ERROR ("URDF library not found. Please install it to use with USE_EXTERNAL_URDF or set this flag to false to use internal URDF code")
      else()
        # urdfdom library found < 0.3, unset flag
        set (URDF_GE_0P3 FALSE)
      endif()

    else()
      # urdfdom library found >= 0.3, set flag
      set (URDF_GE_0P3 TRUE)
    endif()

    # debug
    message(STATUS "\n\ncheck include dirs:\n${URDF_INCLUDEDIR}\n\n")
    message(STATUS "\n\ncheck lib dirs:\n${URDF_LIBDIR}\n\n")
    # what am I doing here? pkg-config and cmake
    set(URDF_INCLUDE_DIRS ${URDF_INCLUDEDIR})
    set(URDF_LIBRARY_DIRS ${URDF_LIBDIR})
  endif()
endif()
