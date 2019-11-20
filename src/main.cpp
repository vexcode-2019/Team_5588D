#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

using namespace okapi::literals;

void disabled() {}

void competition_initialize() {
  /* this funciton runs immediatly after initialization when plugged into a
   * competetion / tournament switch
   *
   * setting this will bring the thread defined in screenController.cpp to
   * autonomous selection mode
  **/
  robot::screen::state = screenMode::selection;
}

void autonomous() {
  /* this function is run when directed by a competition / tournament switch
   * this will follow the function pointer "run" to the selected
   * AutonomousRoutines object. All of these routines are defined in routines.cpp
  **/

  routines[selection].run();
}

void opcontrol() {
  robot::chassis->getModel()->setMaxVelocity(200);
  /* this function runs when directed by a competition / tournament switch
   * or when not connected to either
  **/

  // set the screen to display notification (random text for the time being)
  robot::screen::state = screenMode::notification;

  // disable the profile controller, as the driver will control it manually
  if (robot::chassisProfiler) robot::chassisProfiler->flipDisable(true);

  okapi::ControllerButton buttonL2 = robot::controller[okapi::ControllerDigital::L2];

  robot::towerBar->lower();

  while(true){
    if(robot::controller.getDigital(okapi::ControllerDigital::A)){
      robot::chassis->getModel()->arcade(robot::controller.getAnalog(okapi::ControllerAnalog::leftY) * 0.5, 0);
    }else{
      robot::chassis->getModel()->tank(robot::controller.getAnalog(okapi::ControllerAnalog::leftY),
                                       robot::controller.getAnalog(okapi::ControllerAnalog::rightY));
    }

    if(robot::controller.getDigital(okapi::ControllerDigital::R1)){
      robot::intake->moveVoltage(12000);

    }else if(robot::controller.getDigital(okapi::ControllerDigital::R2)){
      robot::intake->moveVoltage(6000);

    }else if(robot::controller.getDigital(okapi::ControllerDigital::Y)){
      robot::intake->moveVoltage(-6000);
      
    }else{
      robot::intake->moveVoltage(0);
    }

    if(robot::controller.getDigital(okapi::ControllerDigital::right)){
      robot::angler->stack();

    }else if(robot::controller.getDigital(okapi::ControllerDigital::L1)){
      robot::angler->reset();

    }else if(robot::controller.getDigital(okapi::ControllerDigital::down)){
      robot::angler->getTask()->suspend();
      robot::angler->getMotor()->moveVoltage(-12000);

    }else if(robot::controller.getDigital(okapi::ControllerDigital::left)){
      robot::angler->getTask()->resume();
      robot::angler->tare();
    }

    if(buttonL2.changed()){
      if(buttonL2.isPressed()){
        if(robot::towerBar->isRaised()){
          robot::towerBar->lower();
        }else{
          robot::towerBar->intake();
        }
      }else{
        if(robot::towerBar->isRaised()){
          robot::towerBar->raise();
        }else{
          robot::towerBar->lower();
        }
      }
    }

    pros::delay(10);
  }
}
