list(APPEND ALL_LIBRARIES ${PYTHON_LIBRARIES})
list(APPEND ALL_INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_DIRS})
if(MITK_USE_Numpy)
  include(${CMAKE_CURRENT_LIST_DIR}/MITK_Numpy_Config.cmake)
endif()