# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ali/buddy/laptop-software/kinect-image/Color_tracking/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build

# Utility rule file for opencv_test_generate_messages_py.

# Include the progress variables for this target.
include opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/progress.make

opencv_test/CMakeFiles/opencv_test_generate_messages_py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/_position.py
opencv_test/CMakeFiles/opencv_test_generate_messages_py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/__init__.py

/home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/_position.py: /opt/ros/hydro/share/genpy/cmake/../../../lib/genpy/genmsg_py.py
/home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/_position.py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg/position.msg
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating Python from MSG opencv_test/position"
	cd /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && ../catkin_generated/env_cached.sh /usr/bin/python /opt/ros/hydro/share/genpy/cmake/../../../lib/genpy/genmsg_py.py /home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg/position.msg -Iopencv_test:/home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/msg -Isensor_msgs:/opt/ros/hydro/share/sensor_msgs/cmake/../msg -Istd_msgs:/opt/ros/hydro/share/std_msgs/cmake/../msg -Igeometry_msgs:/opt/ros/hydro/share/geometry_msgs/cmake/../msg -p opencv_test -o /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg

/home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/__init__.py: /opt/ros/hydro/share/genpy/cmake/../../../lib/genpy/genmsg_py.py
/home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/__init__.py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/_position.py
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating Python msg __init__.py for opencv_test"
	cd /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && ../catkin_generated/env_cached.sh /usr/bin/python /opt/ros/hydro/share/genpy/cmake/../../../lib/genpy/genmsg_py.py -o /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg --initpy

opencv_test_generate_messages_py: opencv_test/CMakeFiles/opencv_test_generate_messages_py
opencv_test_generate_messages_py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/_position.py
opencv_test_generate_messages_py: /home/ali/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/python2.7/dist-packages/opencv_test/msg/__init__.py
opencv_test_generate_messages_py: opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/build.make
.PHONY : opencv_test_generate_messages_py

# Rule to build all files generated by this target.
opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/build: opencv_test_generate_messages_py
.PHONY : opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/build

opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/clean:
	cd /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && $(CMAKE_COMMAND) -P CMakeFiles/opencv_test_generate_messages_py.dir/cmake_clean.cmake
.PHONY : opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/clean

opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/depend:
	cd /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ali/buddy/laptop-software/kinect-image/Color_tracking/src /home/ali/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test /home/ali/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : opencv_test/CMakeFiles/opencv_test_generate_messages_py.dir/depend

