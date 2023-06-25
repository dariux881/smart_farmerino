#ifndef _MOTOR_MOVEMENTMANAGER_H
#define _MOTOR_MOVEMENTMANAGER_H

#include "MovementManager.h"
#include "A4988.h"

/*
 * Definition and motors' configuration
 */
#define X_STEPS_IN_REVOLUTION 200
#define X_MOTOR_SPEED 200 // RPM
#define X_DIR_PIN 8
#define X_STEP_PIN 9
#define X_EN_PIN 7

#define Y_STEPS_IN_REVOLUTION 200
#define Y_MOTOR_SPEED 200 // RPM
#define Y_DIR_PIN 5
#define Y_STEP_PIN 6
#define Y_EN_PIN 4

#define Z_STEPS_IN_REVOLUTION 200
#define Z_MOTOR_SPEED 70 // RPM
#define Z_DIR_PIN 0
#define Z_STEP_PIN 1

/*
 * Definition how many steps are in a centimeter (unit). Depends on the motor + gears configuration
 */
#define X_STEPS_PER_CM 1 // x-axis
#define Y_STEPS_PER_CM 1 // y-axis
#define Z_STEPS_PER_CM 1 // z-axis 

class MotorMovementManager : public MovementManager
{
    public:
        MotorMovementManager(void (*partialResult) (char* value))
        {
            _partialResult = partialResult;
            InitializeMotors();
            InitializePosition();
        }

        /// @brief Stops all the motors
        void StopAllMotors() {
            ResetSpeed();
        }

        /// @brief Moves the main device to horizontal grid position
        /// @param x x-axis coordinate
        /// @param y y-axis coordinate
        int MoveToXY(float x, float y) {
            if (x != _x) {
                if (!_xMotor) return MOTOR_ERROR;
                
                short xStepFactor = (x > _x) ? 1 : -1;
    
                _currentMovementError = false;
    
                int xSteps = xStepFactor * abs(_x - x) * X_STEPS_PER_CM;
                
                _xMotor->enable();
                _xMotor->move(xSteps);
                _xMotor->disable();
    
                if (_currentMovementError) {
                  return ERROR_GENERIC;
                }
    
                _x = x;              
            }

            if (y != _y) {
                if (!_yMotor) return MOTOR_ERROR;
                
                short yStepFactor = (y > _y) ? 1 : -1;
    
                _currentMovementError = false;

                int ySteps = yStepFactor * abs(_y - y) * Y_STEPS_PER_CM;

                _yMotor->enable();
                _yMotor->move(ySteps);
                _yMotor->disable();
    
                if (_currentMovementError) {
                  return ERROR_GENERIC;
                }
    
                _y = y;
            }

            return NO_ERROR;
        }

        /// @brief Moves device to set height
        /// @param z The desired height
        int MoveToHeight(float z) {
            if (z == _z || !_zMotor) return MOTOR_ERROR;
          
            short zStepFactor = (z > _z) ? 1 : -1;

            _currentMovementError = false;
            _zMotor->enable();
            _zMotor->move(zStepFactor * abs(_z - z) * Z_STEPS_PER_CM);
            _zMotor->disable();

            if (_currentMovementError) {
              return ERROR_GENERIC;
            }

            _z = z;

            return NO_ERROR;
        }

        /// @brief Moves to max possible height.
        /// @return the measured height in centimeters
        int MoveToMaxHeight() {
            if (!_zMotor) return MOTOR_ERROR;
            
            delay(4000);
            _z = 15; //TODO fake
            return (int)_z;
        }

        /// @brief Turns the device on the horizontal plane
        /// @param alpha The desired angle
        int TurnHorizontalPlane(float alpha) {
            delay(1000);
            _alpha = alpha;

            return NO_ERROR;
        }

        /// @brief Turns the device on the vertical plane
        /// @param angle The desired angle
        int TurnVerticalPlane(float beta) {
            delay(1000);
            _beta = beta;

            return NO_ERROR;
        }

    private:
        void (*_partialResult) (char* value);
        A4988* _xMotor;
        A4988* _yMotor;
        A4988* _zMotor;

        void InitializeMotors()
        {
            _xMotor = new A4988(X_STEPS_IN_REVOLUTION, X_DIR_PIN, X_STEP_PIN, X_EN_PIN);
            _xMotor->setEnableActiveState(LOW);
            _xMotor->begin(X_MOTOR_SPEED, 1);

            _yMotor = new A4988(Y_STEPS_IN_REVOLUTION, Y_DIR_PIN, Y_STEP_PIN, Y_EN_PIN);
            _yMotor->setEnableActiveState(LOW);
            _yMotor->begin(Y_MOTOR_SPEED, 1);

            _zMotor = new A4988(Z_STEPS_IN_REVOLUTION, Z_DIR_PIN, Z_STEP_PIN);
            //_zMotor->setEnableActiveState(LOW);
            _zMotor->begin(Z_MOTOR_SPEED, 1);
        }

        void InitializePosition() {
            //TODO init position to all 0
            _x = _y = _z = _alpha = _beta = 0;
        }

        void ResetSpeed() {
        }
};

#endif
