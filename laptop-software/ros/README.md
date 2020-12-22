
# README
How to run ROS Docker-Compose for Buddy Bot
## Pre-Requsites:
 1. Install Docker  
```
sudo apt install docker.io
```
or get from https://docs.docker.com/docker-for-windows/

 2. Install Python3
```
sudo apt install python3 python3-pip
```
or get from https://www.activestate.com/products/python/downloads/

 3. Install docker-compose
```
sudo python3 -m pip install docker-compose
```

 4. On Windows, additional software is needed for UI:https://sourceforge.net/projects/vcxsrv/. Run `laptop-software/ros/config.xlaunch` to start Xserver with the appropriate settings
 5. In terminal (Windows terminal with Windows Subspace for Linux with Ubuntu 20.04 is what I use on windows, bash in Linux) go to the `laptop-software/ros` directory and to build everything run:
```
docker-compose build
```
and to run everything run
```
docker-compose up
```
6. To get a ROS shell, you need to docker exec into the Buddy ROS container. You can do that by running
```
docker exec -it ros_buddy_1 bash
```
and then to initialize the ROS environment run
```
source /home/<user>/ros/devel/setup.bash
```

## Docker-Compose explained
Some things to note about the docker-compose:
 - There are 2 services running in the compose, the `ros-master` and `buddy`. `ros-master` is the head container that ROS is running in while `buddy` is running the core Buddy Bot software. This can be consolidated down to one service or split across multiple if desired.
 - During the `build` process for the `buddy` container image, the user that is running the `build` command will be created inside the container with UID `1000`. This is so that the same user has the same permissions inside and outside the container (ex, user `buddy` with UID `1000` running `docker-compose build` will create a `ros-buddy` docker image that will run with user `buddy` and UID `1000` so that when it tries to access XServer to draw the UI, the user will have the correct permissions. **NOTE:** You may need to run `xhost +local:root` to allow XServer to accept incoming connections.
 - The `laptop-software/buddy/src/motor_control_drivers` and `laptop-software/buddy/src/sensor_to_laser` source folders are mounted as volumes to the container so live edits can be done on that source code without having to rebuild the container. Likewise the `laptop-software/buddy/src/buddy_launch.launch` file can also be edited on the host on the fly.
 - In order for XServer to work, the `DISPLAY` environement variable must be set to `host.docker.internal:0.0` for a windows host and to `$USER` on Linux hosts.
 - After execing into the container as shown in step 6 above, you can run any docker command you wish just as you would on a machine with ROS natively installed. Ex, running `rqt_graph` will load up the ROS node and topic graph UI.
 - RViz is launched by default in the `buddy_launch.launch` file. If you want to Buddy simulation to navigate to a goal, you can select `2D Nav Goal` on the toolbar and place a goal location and orientation on the RViz map and the Buddy-sim will navigate to the location.
