#include "motor.h"
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace std; 

int main(int argc, char* argv[])
{
    CMotor myMotor;

    string cmd = "en0\n";
    myMotor.writePort(cmd);
    cmd = "SP2000\n";
    myMotor.writePort(cmd);
//    sleep(3);
//    cmd = "v0\n";
//    myMotor.writePort(cmd);

   for (int i=0;i<10;i++){
       cmd = "LR11537\n";
       myMotor.writePort(cmd);
       cmd = "m\n";
       myMotor.writePort(cmd);
       sleep(10);
   }

  return 0; 
}
