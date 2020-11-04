# ros\_myo\_cpp

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)

ROS node interfacing the Myo armband (based on this C++ library for Myo: [brokenpylons/MyoLinux](https://github.com/brokenpylons/MyoLinux). 

## Features

* All the features of the mentioned C++ library
* Since the node is based on roscpp instead of rospy (e.g. this one: [ewok261/ros\_myo](https://github.com/ewok261/ros_myo)), it is much faster and publishing EMG messages can actually achieve the rate of 200 Hz.

## Contributing

Please open an issue if you find bugs or any other deficiencies. Note that by contributing to this repository you irrevocably license your work under the MPL 2.0.

## License

This repository is placed under the MPL 2.0. See LICENSE for more details.
