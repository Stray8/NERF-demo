#include <ros/ros.h>
#include <std_msgs/UInt16.h>
#include <termio.h>
#include <stdio.h>
 

#define KEYCODE_S 0x73
#define KEYCODE_S_CAP 0x53

#define KEYCODE_Q 0x71
#define KEYCODE_Q_CAP 0x51
 
int key_command=0x00;
 
void scanKeyboard()
{
	struct termios new_settings;
	struct termios stored_settings;
    //设置终端参数
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	// only getchar must enter \n the stdin can be read in
	key_command = getchar();
	tcsetattr(0,TCSANOW,&stored_settings);
}
 
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"keyboard_control");
    ros::NodeHandle nh;
 
    ros::Publisher key_publisher = nh.advertise<std_msgs::UInt16>("/key_command",10);
    std_msgs::UInt16 key_c;
 
	while(1){
		scanKeyboard();
		switch(key_command)
		{

			case KEYCODE_S:
				key_c.data=1;
				key_publisher.publish(key_c);
				break;

			case KEYCODE_S_CAP:
				key_c.data = 1;
				key_publisher.publish(key_c);
				break;

			case KEYCODE_Q:
				return 0;
			
			case KEYCODE_Q_CAP:
				return 0;

			default:
				key_c.data=0;
				key_publisher.publish(key_c);
				break;
		}
 
	}
}