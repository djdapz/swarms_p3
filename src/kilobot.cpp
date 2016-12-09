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
	int max_ticks = 30;
	int data_ticks = 15;
	int mag_threshold = .1;
	int compass_deg = 0;
	int next_angle_deg = 0;
	int no_turn_zone = 20;

	//main loop
	void loop()
	{
		compass_deg = radian_to_degree(compass);
		spinup_motors();
		set_motors(0, kilo_turn_right);	
		if(ticks > data_ticks){
			if(movement_mag > mag_threshold){
				int command = choose_direction_to_turn(compass_deg, next_angle_deg);

				if(id == 1){
					printf("^^^^^^^^^^^^^\n\r");
					printf("compass:       %d\n\r", compass_deg);
					printf("next_angle:    %d\n\r", next_angle_deg);

					if(command == 1){
						printf("TURN LEFT \n\r");
					}else if (command == 2){
						printf("TURN RIGHT \n\r");
					}else{
						printf("Move forward \n\r");
					}
				}
			}

			//
			// 	if(command == 1)
			// 	{
			// 		spinup_motors();
			// 		set_motors(kilo_turn_left,0);
			// 	} else if (command == 2)
			// 	{
			// 		spinup_motors();
			// 		set_motors(0,kilo_turn_right);
			// 	}else{
			// 		if(movement_mag > mag_threshold){
			// 			spinup_motors();
			// 			set_motors(50, 50);
			// 		}
			// 	}
			// }
			// else{
			// 	spinup_motors();
			// 	set_motors(0,0);
			// }

		}
		else if(ticks == data_ticks){
			double composite_dir_rad = atan2(running_y, running_x);
			next_angle = composite_dir_rad;
			next_angle_deg = radian_to_degree(next_angle);
			movement_mag = sqrt(running_x*running_x + running_y*running_y);
			if(id ==1){
				printf("=====================\n\r");
				printf("running_x	          %f\n\r", running_x);
				printf("running_y	          %f\n\r", running_y);
				printf("composite_dir_rad	  %f\n\r", composite_dir_rad);
				printf("movement_mag: 	      %f\n\r", movement_mag);
				printf("compass:     	      %d\n\r", compass_deg);
				printf("next_angle:  	      %d\n\r", next_angle_deg);
				printf("ticks:         	      %d\n\r", ticks);
				printf("gravity:              %d\n\r", gravity);
				printf("distance:    	      %d\n\r", distance);
			}

			running_x = 0;
			running_y = 0;
		}else{
			spinup_motors();
			set_motors(0,0);
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
		double force_mag = (double)gravity * 1 * 1 / ((double)distance * (double)distance);
		int compass_deg = radian_to_degree(compass);

		if(id == 1 && ticks <=5){
			set_color(RGB(1,0,0));
		}

		double force_x = force_mag * cos(theta);
		double force_y = force_mag * sin(theta);


		if(ticks <data_ticks){
			running_x = force_x + running_x;
			running_y = force_y + running_x;
		}

	}


	int radian_to_degree(double radians){

		double intermediate = radians;
		intermediate = intermediate/(2*M_PI);
    	intermediate = intermediate * 360;

		if(intermediate <0){
    		return (int)intermediate + 360;
	    }else{
			return (int)intermediate;
	    }
	}

	double degrees_to_radians(int degrees){
		return ((double)degrees- 180)*(2*M_PI)/360;
	}

	int choose_direction_to_turn(int compass_deg, int next_angle_deg){
		if(next_angle_deg == compass_deg){
        	return 0;
	    }
	    else if(compass_deg >= 180){
	        if(next_angle_deg < compass_deg){
	            if(compass_deg + no_turn_zone > 360){
	                if(360 - compass_deg + next_angle_deg < no_turn_zone){
	                    return 0;
	                }
	                else if(next_angle_deg > compass_deg - no_turn_zone){
	                    // if in no turn zone
	                    return 0;
	                } else if(next_angle_deg > compass_deg -180 ){
	                    //turn right
	                    return 2;
	                }else{
	                    return 1;
	                }
	            }else{
	                if(next_angle_deg > compass_deg - no_turn_zone){
	                    // if in no turn zone
	                    return 0;
	                } else if(next_angle_deg > compass_deg -180 ){
	                    //turn right
	                    return 2;
	                }else{
	                    return 1;
	                }
	            }
	        }else{
	            if(compass_deg < 360-no_turn_zone-1){
	                //if our no_turn_zone doesnt drop to zero
	                if(next_angle_deg > compass_deg && next_angle_deg < compass_deg + 20){
	                    //in no_turn_zone
	                    return 0;
	                }else{
	                    //turn left
	                    return 1;
	                }
	            }else{
	                if(next_angle_deg > compass_deg || next_angle_deg < 360 - compass_deg){
	                    // no turn zone
	                    return 0;
	                }else{
	                    //turn left
	                    return 1;
	                }
	            }
	        }
	    }
	    else{
	        //compass_deg < 180
	        if(next_angle_deg > compass_deg){
	            if(compass_deg - no_turn_zone <0){
	                if(360-next_angle_deg+compass_deg < no_turn_zone){
	                    return 0;
	                }else if(next_angle_deg < compass_deg + 180){
	                    //turn left
	                    return 1;
	                }else{
	                    //beyond that zone, turn right
	                    return 2;
	                }
	            }
	            else if((next_angle_deg - no_turn_zone) < compass_deg){
	                // if in no turn zone
	                return 0;
	            } else if(next_angle_deg < compass_deg + 180){
	                //turn left
	                return 1;
	            }else{
	                //beyond that zone, turn right
	                return 2;
	            }
	        }else{
	            //next_angle_deg < compass_deg
	            if(compass_deg - no_turn_zone < 180){
	                //if our no_turn_zone doesnt drop to zero
	                if(next_angle_deg > compass_deg - no_turn_zone){
	                    //in no_turn_zone
	                    return 0;
	                }
	                else{
	                    //turn right
	                    return 2;
	                }
	            }

	            else{
	                //if our compass does drop to zero and the next angle is less
	                //than zero, then the next angle is in the no turn zone
	                // no turn zone
	                return 0;
	            }
	        }
	    }
	}

};
