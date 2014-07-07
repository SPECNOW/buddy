# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "opencv_test: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iopencv_test:/home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg;-Isensor_msgs:/opt/ros/hydro/share/sensor_msgs/cmake/../msg;-Istd_msgs:/opt/ros/hydro/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/hydro/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(opencv_test_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(opencv_test
  "/home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/opencv_test
)

### Generating Services

### Generating Module File
_generate_module_cpp(opencv_test
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/opencv_test
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(opencv_test_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(opencv_test_generate_messages opencv_test_generate_messages_cpp)

# target for backward compatibility
add_custom_target(opencv_test_gencpp)
add_dependencies(opencv_test_gencpp opencv_test_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS opencv_test_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(opencv_test
  "/home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/opencv_test
)

### Generating Services

### Generating Module File
_generate_module_lisp(opencv_test
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/opencv_test
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(opencv_test_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(opencv_test_generate_messages opencv_test_generate_messages_lisp)

# target for backward compatibility
add_custom_target(opencv_test_genlisp)
add_dependencies(opencv_test_genlisp opencv_test_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS opencv_test_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(opencv_test
  "/home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/opencv_test
)

### Generating Services

### Generating Module File
_generate_module_py(opencv_test
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/opencv_test
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(opencv_test_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(opencv_test_generate_messages opencv_test_generate_messages_py)

# target for backward compatibility
add_custom_target(opencv_test_genpy)
add_dependencies(opencv_test_genpy opencv_test_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS opencv_test_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/opencv_test)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/opencv_test
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(opencv_test_generate_messages_cpp sensor_msgs_generate_messages_cpp)
add_dependencies(opencv_test_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/opencv_test)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/opencv_test
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(opencv_test_generate_messages_lisp sensor_msgs_generate_messages_lisp)
add_dependencies(opencv_test_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/opencv_test)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/opencv_test\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/opencv_test
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(opencv_test_generate_messages_py sensor_msgs_generate_messages_py)
add_dependencies(opencv_test_generate_messages_py std_msgs_generate_messages_py)
