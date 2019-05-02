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
  cmd = "v-1000\n";
  myMotor.writePort(cmd);  
  sleep(3);
  cmd = "v0\n";
  myMotor.writePort(cmd);    

  return 0; 
}
