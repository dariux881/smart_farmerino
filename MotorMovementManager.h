#ifndef _MOTOR_MOVEMENTMANAGER_H
#define _MOTOR_MOVEMENTMANAGER_H

#include <Stepper.h>
#include "MovementManager.h"

/*
 * Definition and motors' configuration
 */
#define X_STEPS_IN_REVOLUTION 200
#define X_MOTOR_SPEED 70 // RPM
#define X_PIN_1 1
#define X_PIN_2 2
#define X_PIN_3 3
#define X_PIN_4 4

#define Y_STEPS_IN_REVOLUTION 200
#define Y_MOTOR_SPEED 70 // RPM
#define Y_PIN_1 1
#define Y_PIN_2 2
#define Y_PIN_3 3
#define Y_PIN_4 4

#define Z_STEPS_IN_REVOLUTION 200
#define Z_MOTOR_SPEED 70 // RPM
#define Z_PIN_1 1
#define Z_PIN_2 2
#define Z_PIN_3 3
#define Z_PIN_4 4

/*
 * Definition how many steps are in a centimeter (unit). Depends on the motor + gears configuration
 */
#define X_STEPS_PER_CM 10 // x-axis
#define Y_STEPS_PER_CM 10 // y-axis
#define Z_STEPS_PER_CM 10 // z-axis 

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
            _xStep->setSpeed(0);
            _yStep->setSpeed(0);
            _zStep->setSpeed(0);
        }

        /// @brief Moves the main device to horizontal grid position
        /// @param x x-axis coordinate
        /// @param y y-axis coordinate
        int MoveToXY(float x, float y) {
            short xStepFactor = (x > _x) ? 1 : -1;

            _currentMovementError = false;
            // enable motors
            _xStep->step(xStepFactor * abs(_x - x) * X_STEPS_PER_CM);

            if (_currentMovementError) {
              return ERROR_GENERIC;
            }

            _x = x;

            short yStepFactor = (y > _y) ? 1 : -1;
            // enable motors
            _yStep->step(yStepFactor * abs(_y - y) * Y_STEPS_PER_CM);

            if (_currentMovementError) {
              return ERROR_GENERIC;
            }

            _y = y;

            return NO_ERROR;
        }

        /// @brief Moves device to set height
        /// @param z The desired height
        int MoveToHeight(float z) {
            short zStepFactor = (z > _z) ? 1 : -1;

            _currentMovementError = false;
            // enable motors
            _zStep->step(zStepFactor * abs(_z - z) * Z_STEPS_PER_CM);

            if (_currentMovementError) {
              return ERROR_GENERIC;
            }

            _z = z;

            return NO_ERROR;
        }

        /// @brief Moves to max possible height.
        /// @return the measured height in centimeters
        int MoveToMaxHeight() {
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
        float _x, _y, _z, _alpha, _beta;
        bool _currentMovementError;
        Stepper* _xStep;
        Stepper* _yStep;
        Stepper* _zStep;

        void InitializeMotors()
        {
            _xStep = new Stepper(X_STEPS_IN_REVOLUTION, X_PIN_1, X_PIN_2, X_PIN_3, X_PIN_4);
            _xStep->setSpeed(X_MOTOR_SPEED);

            _yStep = new Stepper(Y_STEPS_IN_REVOLUTION, Y_PIN_1, Y_PIN_2, Y_PIN_3, Y_PIN_4);
            _yStep->setSpeed(Y_MOTOR_SPEED);

            _zStep = new Stepper(Z_STEPS_IN_REVOLUTION, Z_PIN_1, Z_PIN_2, Z_PIN_3, Z_PIN_4);
            _zStep->setSpeed(Z_MOTOR_SPEED);
        }

        void InitializePosition() {
            //TODO init position to all 0
            _x = _y = _z = _alpha = _beta = 0;
        }
};

#endif
