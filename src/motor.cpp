/*
 *  Apr. 21 2017
 *  Modified by He Zhang, hxzhang1@ualr.edu 
 *  
 *  TODO: adjust rotate velocity 
 *
 * */

#include "motor.h"

using namespace std; 
CMotor::CMotor(double t, std::string port) : 
  PORT(port), 
  Baud(t), 
  maxspeed(9999)
{
  std::cout << "The motor is=> Baud "<< Baud<<" Port "<< PORT<<std::endl;
  _motor.Open(PORT);		

  if ( ! _motor.good() ){
    std::cerr << "Error: Could not open serial port "<<PORT<< std::endl ;
    exit(1) ;
  }

  if(Baud==115200){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
    std::cout<<"Going for value of 115200"<<std::endl;
  }
  else if(Baud==1200){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_1200);
    std::cout<<"Going for value of 1200"<<std::endl;
  }
  else if(Baud==2400){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_2400);
    std::cout<<"Going for value of 2400"<<std::endl;
  }
  else if(Baud==19200){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_19200);
    std::cout<<"Going for value of 19200"<<std::endl;
  }
  else if(Baud==38400){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_38400);
    std::cout<<"Going for value of 38400"<<std::endl;
  }
  else if(Baud==57600){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_57600);
    std::cout<<"Going for value of 57600"<<std::endl;
  }
  else if(Baud==9600){
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
    std::cout<<"Going for default value of 9600"<<std::endl;
  }
  else{
    std::cout<<"Value incorrect. Going for default value of 9600"<<std::endl;
    _motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
  }

  if ( ! _motor.good() ){
    std::cerr << "Error: Could not set the baud rate." <<std::endl;
    exit(1);
  }		


  _motor.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
  if ( ! _motor.good() ){
    std::cerr << "Error: Could not set the char size." <<std::endl;
    exit(1);
  }
  _motor.SetParity( LibSerial::SerialStreamBuf::PARITY_NONE ) ;
  if ( ! _motor.good() ){
    std::cerr << "Error: Could not set the parity." <<std::endl;
    exit(1);
  }
  _motor.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE ) ;
  if ( ! _motor.good() ){
    std::cerr << "Error: Could not set the control." <<std::endl;
    exit(1);
  }
  _motor.SetNumOfStopBits(1) ;
  if ( ! _motor.good() ){
    std::cerr << "Error: Could not set the stopbit." <<std::endl;
    exit(1);
  }

  //setMax Min speed
  writeDisable(); //Stop the motor in case the robot was still moving
  writeEnable();
  writeHome(); //Set the initial position to zero ;)

  writeTargetSpeed(50);
  writeMaxSpeed(maxspeed);

}

CMotor::~CMotor(){
  _motor.Close();
}


void CMotor::writeMaxSpeed(int ms){
  try{
    std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
    MaxSpeed="SP"+MaxSpeed+"\n";
    writePort(MaxSpeed);
  }
  catch(boost::bad_lexical_cast& blc){
    std::cout << "Exception in Max Speed" << blc.what() << std::endl;
    _readcorrectly=false;
    //	  scanf("%d",&testin);
  }
}
void CMotor::writeMinSpeed(int ms){
  try{
    std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
    MaxSpeed="MV"+MaxSpeed+"\n";
    writePort(MaxSpeed);
  }
  catch(boost::bad_lexical_cast& blc){
    std::cout << "Exception in Min Speed" << blc.what() << std::endl;
    //	  scanf("%d",&testin);
    _readcorrectly=false;
  }
}

void CMotor::writeTargetSpeed(int ms){
  try{
    std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
    MaxSpeed="V"+MaxSpeed+"\n";
    writePort(MaxSpeed);
  }
  catch(boost::bad_lexical_cast& blc){
    std::cout << "Exception in Right wheel Speed" << blc.what() << std::endl;
    //	  scanf("%d",&testin);
    _readcorrectly=false;
  }
}


bool CMotor::motorMove(char targetDirt, int targetPos, int targetSpeed)
{
  std::string cmdSpeed=boost::lexical_cast<std::string>(targetSpeed);
  int curPos = readEncoder();
  std::cout << "curPos = "<< curPos<<std::endl;
  bool useV = false;
  std::string cmd;
  if (useV){
    if (targetDirt == 'l'){
      cmd ="V"+cmdSpeed+"\n";
      targetPos = curPos + targetPos;
    }
    else{
      cmd ="V-"+cmdSpeed+"\n";
      targetPos = curPos - targetPos;
    }
    writePort(cmd);
  }
  else{
    if (targetDirt == 'l'){
      targetPos = curPos + targetPos;
    }
    else{
      targetPos = curPos - targetPos;
    }
    std::string cmdPos=boost::lexical_cast<std::string>(targetPos);
    cmd ="SP"+cmdSpeed+"\n";
    writePort(cmd);
    cmd ="LA"+cmdPos+"\n";
    writePort(cmd);
    cmd ="M\n";
    writePort(cmd);  
  }
  //std::cout << "CMD = "<< pos<<std::endl;
  //sleep(4);
  
  //std::cout << "curPos = "<< curPos<<std::endl;
  //std::cout << "targetPos = "<< targetPos<<std::endl;
  int stopCode = 0;
  int lastPos = 0;
  while(abs(curPos-targetPos)>10 )
  {
    lastPos = curPos;
    usleep(50);
    curPos = readEncoder();
    // std::cout << "curPos = "<< curPos<<std::endl;
    // std::cout << "lastPos = "<< lastPos<<std::endl;
    // std::cout << "curPos-targetPos = "<< (curPos-targetPos)<<std::endl;


    if(abs(curPos-lastPos)<2 ){
      stopCode ++;
    }
    else{
      stopCode = 0;
    }

    if(abs(curPos-targetPos)<10){
      cmd="V0\n";
      writePort(cmd);
      std::cout << "Stopped!!!"<< std::endl;
      std::cout << "curPos = "<< curPos<<std::endl;
      if (abs(curPos-targetPos)>10 ){
        std::cout << "abs(curPos-targetPos) = "<< abs(curPos-targetPos)<<std::endl;
        std::cout << "Stuck, failed to move!!!"<< std::endl;
        // for (int i=0 ; i<5; i++){
        //   std::cout << 5-i << std::endl;
        //  sleep(1);
        // }
        return false; 
      }
      // pos="EN0\n";
      // writePort(pos);
      // might need to release the clutch and re-enable the motor
      break;
    }
  }
  return true; 
}


void CMotor::writeMove()
{
  std::string s="M\n";
}




