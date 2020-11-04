/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ros/ros.h"
#include "ros_myo_cpp/EmgArray.h"
#include "sensor_msgs/Imu.h"

#include "myoclient.h"
#include "myoapi.h"
#include "serial.h"

#include <cinttypes>

using namespace myolinux; 

int main(int argc, char **argv)
{
    ros::init(argc, argv, "myo_raw");
    ros::NodeHandle nh("~");
    ros::Publisher emgPub = nh.advertise<ros_myo_cpp::EmgArray>("myo_emg",1);
    ros_myo_cpp::EmgArray emg_message;
    ros::Publisher imuPub = nh.advertise<sensor_msgs::Imu>("myo_imu",1);
    sensor_msgs::Imu imu_message;
	
    myo::Client client(Serial{"/dev/ttyACM0", 115200});

    // Autoconnect to the first Myo device
    client.connect();
    if (!client.connected()) {
        return 1;
    }

    // Print device address
    print_address(client.address());

    // Read firmware version
    auto version = client.firmwareVersion();
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    client.vibrate(myo::Vibration::Medium);

    // Read name
    auto name = client.deviceName();
    std::cout << name << std::endl;

    // Set sleep mode
    client.setSleepMode(myo::SleepMode::NeverSleep);

    // Read EMG and IMU
    client.setMode(myo::EmgMode::SendEmg, myo::ImuMode::SendData, myo::ClassifierMode::Disabled);

    client.onEmg([&emg_message,emgPub](myo::EmgSample sample)
    {
        for (std::size_t i = 0; i < 8; i++) 
	{
	    emg_message.data[i] = sample[i];
        }
	emgPub.publish(emg_message);
    });

    client.onImu([&imu_message, imuPub](myo::OrientationSample ori, myo::AccelerometerSample acc, myo::GyroscopeSample gyr)
    {
        imu_message.orientation.x = ori[1]/myo::OrientationScale;
	imu_message.orientation.y = ori[2]/myo::OrientationScale;
	imu_message.orientation.z = ori[3]/myo::OrientationScale;
	imu_message.orientation.w = ori[0]/myo::OrientationScale;
	imu_message.linear_acceleration.x = acc[0]/myo::AccelerometerScale;
	imu_message.linear_acceleration.y = acc[1]/myo::AccelerometerScale;
	imu_message.linear_acceleration.z = acc[2]/myo::AccelerometerScale;
	imu_message.angular_velocity.x = gyr[0]/myo::GyroscopeScale;
	imu_message.angular_velocity.y = gyr[1]/myo::GyroscopeScale;
	imu_message.angular_velocity.z = gyr[2]/myo::GyroscopeScale;

	imuPub.publish(imu_message);
    });

    try 
    {
        while (ros::ok()) 
	{
            client.listen();
        }
    }
    catch(myo::DisconnectedException &) 
    {
        std::cout << "Disconnected" << std::endl;
    }
}
