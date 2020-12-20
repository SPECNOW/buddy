
FROM osrf/ros:kinetic-desktop-full

RUN useradd -ms /bin/bash buddy

RUN apt update -y 
RUN apt install -y python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential python-pip
RUN python -m pip install serial

#COPY buddy /home/buddy/ros
USER buddy
WORKDIR /home/buddy/
COPY entrypoint.sh .
#ENTRYPOINT roslaunch buddy_launch.launch
CMD ["/bin/bash", "/home/buddy/entrypoint.sh"]