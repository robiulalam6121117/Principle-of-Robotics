/*
 *  bumper.cc
 * 
 *  Sample code for a robot that has two front bumpers and uses them to
 *  avoid obstacles. Suitable for use with the Roomba and Create. 
 * 
 *  Works IRL. Doesn't work so well in Stage, probably because of the
 *  crude modeling of the bumpers, and can get stuck in convex shapes
 *  when it backs into an object while trying to turn away from it.
 *
 *  Based on an example provided by Monica Anderson and Jeff Forbes,
 *  via Carlos Jaramillo, and changed to (hopefully) make it easier to
 *  understand.
 *
 *  Modified:    Simon Parsons
 *  Date:        15th June 2009
 *  Last change: 19th September 2011
 *  
 */

#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[])
{  
  using namespace PlayerCc;  

  // Set up proxies. Proxies are the datastructures that Player uses to
  // talk to the simulator and the real robot.

  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);       // The bumper proxy reads from the
                                      // bumpers.
  Position2dProxy pp(&robot,0);       // The 2D proxy reads odometry from 
                                      // the robot and sends motion commands.
  bool shouldGoHome = false;
  bool IsMotorEnable = true;   

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  const double EPS = 0.05;

  // Control loop
  while(IsMotorEnable) 
    {    
      double turnrate, speed;

      // Read from the proxies.
      robot.Read();

      // What does odometry tell us? In other words, how far do we
      // think we have gone?
      std::cout << "x: " << pp.GetXPos()  << std::endl;
      std::cout << "y: " << pp.GetYPos()  << std::endl;
      std::cout << "a: " << pp.GetYaw()  << std::endl;


      // Print out what the bumpers tell us:
      std::cout << "Left  bumper: " << bp[0] << std::endl;
      std::cout << "Right bumper: " << bp[1] << std::endl;
      
      // If  the robot get stall than it will randomly move and turn.
      if(pp.GetStall())
      {
       if(rand()%2 > 0){
	    turnrate = dtor(10);
	  }
	  else {
	    turnrate = dtor(-10);
	  } 
	if(rand()%2 > 0){
	    speed=-0.5;
	  }
	  else {
	    speed=0.5;
	  }
      }

      // If either bumper is pressed, go backwards. Depending on the
      // combination of bumpers that are pressed, turn some also,
      // trying to turn away from the point of contact. 
      //
      // Otherwise just go forwards
      else if(bp[0] || bp[1]){
      
       	   speed=-0.5;

 
	// Left bumper in contact, right bumper not in contact. Turn
	// to the right.  
	//
	// dtor converts from degrees to radians.
	if (bp[0] && !bp[1]) { 
	  turnrate=dtor(-10);  
	}

	// Left bumper not in contact, right bumper in contact. Turn
	// to the left
	if (!bp[0] && bp[1]) {
	  turnrate=dtor(10);
	}

	// Both left and right bumpers in contact. Make a random
	// choice about which way to turn.
	if (bp[0] && bp[1]) 
	{
	    turnrate=dtor(10);
	}
     }
     // robot move forward, when it's  head point to  downward, it start to move  to Point (0,0)
     else if(!shouldGoHome)
      {
      turnrate = 0;
      speed=0.5;
          if(pp.GetYaw()<-1.0 && pp.GetYaw()>-2.0)
          {
              shouldGoHome = true;
      	  }
       }
      else
      {
	  // move forward unitl Y position equal to 0 then stop moving.
	  if (fabs(pp.GetYPos() - 0) < EPS)
	  {   
	       //  turn robot's head to forward to the positive  position of X axis then move foward.
		if(fabs(pp.GetYaw()-0) < EPS)
		{
          	        // move forward unitl X position also equal to 0 then stop the robot.
			if(fabs(pp.GetXPos() - 0) < EPS){
				IsMotorEnable =false;

			}
			else 
			{
				turnrate = 0;
    	   		        speed = 0.5;

			}
			
                }
		else
		{
			turnrate = dtor(10);
			speed = 0;
		}


	    }
   	    else
    	    {
        	turnrate = 0;
    	        speed = 0.5;
            }
        }
        
   


      // What did we decide to do?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
      // Send the motion commands that we decided on to the robot.
      pp.SetSpeed(speed, turnrate);  
    }

}


