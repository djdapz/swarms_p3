#pragma once
#include "kilolib.h"

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;
	float theta;
	float next_angle;

	int gravity = 1200;
	int raidus_goal = 50;

	int motion=0;
	long int motion_timer=0;

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;

	};

	double running_x = 0;
	double running_y = 0;
	double movement_mag = 0;
	int ticks = 0;
	int max_ticks = 14;
	int mag_threshold = 1000;


	//main loop
	void loop()
	{

		if(ticks >=11){
			if(movement_mag > mag_threshold){
				spinup_motors();
				set_motors(50, 50);
			}
		}else if(ticks > 6){
			// if(id==0)
			// {
			//
			// 	if(fabs(theta)<.3)
			// 	{
			//
			// 		spinup_motors();
			// 		set_motors(50,50);
			//
			// 	}
			// 	else if(theta<0)
			// 	{
			// 		spinup_motors();
			// 		set_motors(kilo_turn_left,0);
			//
			//
			//
			// 	}
			// 	else
			// 	{
			// 		spinup_motors();
			// 		set_motors(0,kilo_turn_right);
			//
			//
			// 	}
			// }
			// else
			// {
			// 	//printf("compass =%f\n\r",compass);
			//
			// 	if(fabs(compass-1.5)<.1)
			// 	{
			//
			// 		spinup_motors();
			// 		set_motors(50,50);
			//
			// 	}
			// 	else if(compass-1.5<0)
			// 	{
			// 		spinup_motors();
			// 		set_motors(kilo_turn_left,0);
			//
			//
			//
			// 	}
			// 	else
			// 	{
			// 		spinup_motors();
			// 		set_motors(0,kilo_turn_right);
			//
			//
			// 	}
			//
			// }
			if(movement_mag > mag_threshold){
				if(fabs(next_angle)<.3)
				{

				}
				else if(next_angle<0)
				{
					spinup_motors();
					set_motors(kilo_turn_left,0);
				}
				else
				{
					spinup_motors();
					set_motors(0,kilo_turn_right);
				}
			}

		}else if(ticks == 6){
			double composite_dir_rad = atan2(running_y, running_x);
			next_angle = composite_dir_rad;
			movement_mag = sqrt(running_x*running_x + running_y*running_y);
			// if(id ==1){
			// 	std::cout<<"====================="<<std::endl;
			// 	std::cout<<"running_x	:  " << running_x<<std::endl;
			// 	std::cout<<"running_y	:  " << running_y<<std::endl;
			// 	std::cout<<"movement_mag:  " << movement_mag<<std::endl;
			// }

			running_x = 0;
			running_y = 0;
		}

		if(ticks == max_ticks){
			ticks = 0;
		}else{
			ticks ++;
		}


	}

	//executed once at start
	void setup()
	{

		out_message.type = NORMAL;
		out_message.crc = message_crc(&out_message);
		set_color(RGB(0,1,0)); //starting color doesn't matter
	}

	//executed on successfull message send
	void message_tx_success()
	{
		//set_color(RGB(1,0,0));

	}

	//sends message at fixed rate
	message_t *message_tx()
	{
		static int count = rand();
		count--;
		if (!(count % 10))
		{
			return &out_message;
		}
		return NULL;
	}

	// //receives message
	// void message_rx(message_t *message, distance_measurement_t *distance_measurement,float t)
	// {
	// 	distance = estimate_distance(distance_measurement);
	// 	theta=t;
	//
	//
	//
	// 	//caluclate force vector
	//
	// 	//decide on direction
	// 	if(distance < raidus_goal){
	// 		//reverse theta
	// 		theta = radian_to_degree(t);
	// 		theta += 180;
	// 		theta = degrees_to_radians(theta);
	// 	}
	//
	// 	//decide on magnitued
	// 	double force_mag = gravity * 1 * 1 / (distance * distance);
	//
	// 	if(id == 1 && ticks <=5){
	// 		set_color(RGB(1,0,0));
	// 		std::cout<<"============="<<'\n';
	// 		std::cout<<"force_mag: "<<force_mag<<'\n';
	// 		std::cout<<"ticks: "<<ticks<<'\n';
	// 		std::cout<<"gravity: "<<gravity<<'\n';
	// 		// std::cout<<"distance: "<<distance<<'\n';
	// 		fprintf("dist %d\n\r", distance);
	// 	}
	//
	//
	// 	double force_x = force_mag * cos(theta);
	// 	double force_y = force_mag * sin(theta);
	//
	//
	// 	if(ticks <6){
	// 		running_x = force_x + running_x;
	// 		running_y = force_y + running_x;
	// 	}
	// }


	void message_rx(message_t *message, distance_measurement_t *distance_measurement,float t)

	{

        distance = estimate_distance(distance_measurement);
		theta = t;

		//decide on direction
		if(distance < raidus_goal){
			//reverse theta
			theta = radian_to_degree(t);
			theta += 180;
			theta = degrees_to_radians(theta);
		}

		//decide on magnitued
		double force_mag = gravity * 1 * 1 / (distance * distance);

		if(id == 1 && ticks <=5){
			set_color(RGB(1,0,0));
			std::cout<<"============="<<'\n';
			std::cout<<"force_mag: "<<force_mag<<'\n';
			std::cout<<"ticks: "<<ticks<<'\n';
			std::cout<<"gravity: "<<gravity<<'\n';
			// std::cout<<"distance: "<<distance<<'\n';
			printf("=============\n\r");
			printf("force_mag:  %d\n\r",force_mag);
			printf("ticks:      %d\n\r",ticks);
			printf("gravity:    %d\n\r",gravity);
			printf("distance:   %d\n\r",distance);
			printf("force_mag:  %d\n\r",force_mag);
		}



	}


	int radian_to_degree(double radians){
		return (int)radians * 360 / (2*M_PI) + 180;
	}

	double degrees_to_radians(int degrees){
		return ((double)degrees- 180)*(2*M_PI)/360;
	}
};
