/*
 *  blobs.cc
 * 
 *  The beginnings of a controller that uses the blobfinder.
 *
 *  Created:     Simon Parsons
 *  Date:        7th October 2011
 *  
 */


#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

int main(int argc, char *argv[])
{  
  using namespace PlayerCc;  

  // Set up proxies
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  PlayerClient    blobfinder("localhost");
  BlobfinderProxy bf(&blobfinder,0);

  // Variables

  player_blobfinder_data_t  blobList;   // Structure holding the blobs found
  player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
  player_blobfinder_blob_t  myBlob;     // A single blob
  player_blobfinder_blob_t  emptyBlob;  //a const empty b
  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  const double EPS  = 0.5;  //acceptalbe error for  double  completing
  // Control loop
  while(true) 
    {    
      double turnrate, speed;

      // Read from the proxies
      robot.Read();
      blobfinder.Read();

      // For safety, in the absence of a controller, do nothing.
      speed = 0;
      turnrate = 0;

      // We only want to drive if the bumpers are not pressed
//     if(myBlob == NULL){	      std::cout << "dasdad: "   << std::endl;}
      if ((bp[0] == 0)&&(bp[1] == 0))
	{
	  if (bf.GetCount() > 0){

	    // Here the blobfinder has found some color blobs

	    std::cout << std::endl;
	    std::cout << bf.GetCount() << " Blobs found!" << std::endl;
	    
	    for(int i = 0; i < bf.GetCount(); i++){
	      std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;
	      std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl;
	      std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
	      std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
	      std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
	      std::cout << std::endl;
              
             //find th lagest blobs 
	      int j = i + 1;
              if(j <= ((int)bf.GetCount() -1)){
	        if(bf.GetBlob(i).area > bf.GetBlob(j).area){
			 myBlob = bf.GetBlob(i);
		}
		else{ 
			myBlob= bf.GetBlob(j);
		}    
	      }

	    }// end of loop
            	      std::cout << "myBlobArea: "  << myBlob.area  << std::endl;
            if(myBlob.area > 300 && myBlob.x > 150 && myBlob.x < 170 )
	    {
		
	        turnrate = 0;
	        speed = 1000 / (double)myBlob.area ;
		if(speed > 1) speed = 0.8; //limit  the max speed to 0.8
		if(speed < 0.1) speed = 0.1;//limit  the min speed to 0.1
		      std::cout << "Speed1: " << speed << std::endl;      
	     }
             else{
               speed = 0;
	       turnrate = dtor(30);
	     }

	   //turn the robot point to the  lager blob.
             

	  }
	  else
	    {
	      //If blobfinder doesn;t see any blobs, turning to find blobs. 
	      turnrate = dtor(30);
              
	    }
	}
      
      // If bumpers are pressed, do nothing.

      else{
	speed = 0;
	turnrate = 0;
      }


      //std::cout << pp << std::endl;      
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
      
      	      //turnrate = 0;
		//speed = 0;
      pp.SetSpeed(speed, turnrate);
  
    } // end of while(true) loop
}

;
