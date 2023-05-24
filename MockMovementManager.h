#ifndef _MOCK_MOVEMENTMANAGER_H
#define _MOCK_MOVEMENTMANAGER_H

#include "MovementManager.h"

class MockMovementManager : public MovementManager
{
    public:
        MockMovementManager(void (*partialResult) (char* value))
        {
            _partialResult = partialResult;
            InitializePosition();
        }

        /// @brief Stops all the motors
        void StopAllMotors() {
            Serial.println("Stopping all motors");
        }

        /// @brief Moves the main device to horizontal grid position
        /// @param x x-axis coordinate
        /// @param y y-axis coordinate
        int MoveToXY(float x, float y) {
            char buffer[80];
            sprintf(buffer, "x: %f -> %f\ny: %f -> %f", _x, x, _y, y);
            Serial.println(buffer);

            short xStepFactor = (x > _x) ? 1 : -1;
            short yStepFactor = (y > _y) ? 1 : -1;

            char partialPos[50];

            float xCmUpdate = 5; // increment when update
            float yCmUpdate = 10; // increment when update

            float xVel = 0.01 * xCmUpdate; // m/s
            float yVel = 0.01 * yCmUpdate; // m/s

            while (_x != x)
            {
                float xStepValue = min(xCmUpdate, abs(_x - x));

                delay(xStepValue * xVel * 1000);

                _x += xStepFactor*xStepValue;

                sprintf(partialPos, "%f%s%f", _x, PARAM_SEPARATOR, _y);
                if (_partialResult) _partialResult(partialPos);
            }

            // trace partial movements
            //TODO check if still useful
            while (_y != y)
            {
                float yStepValue = min(yCmUpdate, abs(y - _y));

                delay(yStepValue * yVel * 1000);

                _y += yStepFactor*yStepValue;

                sprintf(partialPos, "%f%s%f", _x, PARAM_SEPARATOR, _y);
                if (_partialResult) _partialResult(partialPos);
            }

            return NO_ERROR;
        }

        /// @brief Moves device to set height
        /// @param z The desired height
        int MoveToHeight(float z) {
            char buffer[40];
            sprintf(buffer, "z: %f->%f", _z, z);
            Serial.println(buffer);

            short zStepFactor = (z > _z) ? 1 : -1;

            // trace partial movements
            //TODO check if still useful
            char partialPos[50];

            float zCmUpdate = 5; // increment when update
            float zVel = 0.01 * zCmUpdate; // m/s

            while (_z != z)
            {
                float zStepValue = min(zCmUpdate, abs(z - _z));

                delay(zStepValue * zVel * 1000);

                _z += zStepFactor * zStepValue;

                sprintf(partialPos, "%f", _z);
                if (_partialResult) _partialResult(partialPos);
            }

            return NO_ERROR;
        }

        /// @brief Moves to max possible height.
        /// @return the measured height in centimeters
        int MoveToMaxHeight() {
            char buffer[40];
            sprintf(buffer, "going to max height");
            Serial.println(buffer);

            delay(4000);
            _z = 15; //TODO fake
            return (int)_z;
        }

        /// @brief Turns the device on the horizontal plane
        /// @param alpha The desired angle
        int TurnHorizontalPlane(float alpha) {
            char buffer[40];
            sprintf(buffer, "going to horizontal angle %f", alpha);
            Serial.println(buffer);

            delay(1000);
            _alpha = alpha;

            return NO_ERROR;
        }

        /// @brief Turns the device on the vertical plane
        /// @param angle The desired angle
        int TurnVerticalPlane(float beta) {
            char buffer[40];
            sprintf(buffer, "going to vertical angle %f", beta);
            Serial.println(buffer);

            delay(1000);
            _beta = beta;

            return NO_ERROR;
        }

    private:
        void (*_partialResult) (char* value);
        float _x, _y, _z, _alpha, _beta;

        void InitializePosition() {
            _x = _y = _z = _alpha = _beta = 0;
        }
};

#endif
