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
CMAKE_SOURCE_DIR = /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build

# Include any dependencies generated for this target.
include opencv_test/CMakeFiles/opencv_test_imageConverter.dir/depend.make

# Include the progress variables for this target.
include opencv_test/CMakeFiles/opencv_test_imageConverter.dir/progress.make

# Include the compile flags for this target's objects.
include opencv_test/CMakeFiles/opencv_test_imageConverter.dir/flags.make

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/flags.make
opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o: /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/src/image_converter.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o"
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o -c /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/src/image_converter.cpp

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.i"
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/src/image_converter.cpp > CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.i

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.s"
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test/src/image_converter.cpp -o CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.s

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.requires:
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.requires

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.provides: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.requires
	$(MAKE) -f opencv_test/CMakeFiles/opencv_test_imageConverter.dir/build.make opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.provides.build
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.provides

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.provides.build: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o

# Object files for target opencv_test_imageConverter
opencv_test_imageConverter_OBJECTS = \
"CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o"

# External object files for target opencv_test_imageConverter
opencv_test_imageConverter_EXTERNAL_OBJECTS =

/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libcv_bridge.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_calib3d.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_contrib.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_core.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_features2d.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_flann.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_gpu.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_highgui.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_imgproc.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_legacy.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_ml.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_nonfree.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_objdetect.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_photo.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_stitching.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_superres.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_video.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libopencv_videostab.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libimage_transport.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libmessage_filters.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libtinyxml.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libclass_loader.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libPocoFoundation.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/x86_64-linux-gnu/libdl.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libroslib.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libroscpp.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_signals-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_filesystem-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/librosconsole.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/librosconsole_log4cxx.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/librosconsole_backend_interface.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/liblog4cxx.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_regex-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libxmlrpcpp.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libroscpp_serialization.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/librostime.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_date_time-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_system-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/libboost_thread-mt.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libcpp_common.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: /opt/ros/hydro/lib/libconsole_bridge.so
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/build.make
/home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter"
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_test_imageConverter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
opencv_test/CMakeFiles/opencv_test_imageConverter.dir/build: /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/devel/lib/opencv_test/opencv_test_imageConverter
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/build

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/requires: opencv_test/CMakeFiles/opencv_test_imageConverter.dir/src/image_converter.cpp.o.requires
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/requires

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/clean:
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test && $(CMAKE_COMMAND) -P CMakeFiles/opencv_test_imageConverter.dir/cmake_clean.cmake
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/clean

opencv_test/CMakeFiles/opencv_test_imageConverter.dir/depend:
	cd /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/src/opencv_test /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test /home/charanpreet/buddy_git/buddy/laptop-software/kinect-image/Color_tracking/build/opencv_test/CMakeFiles/opencv_test_imageConverter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : opencv_test/CMakeFiles/opencv_test_imageConverter.dir/depend

