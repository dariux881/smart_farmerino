#ifndef _MOVEMENTMANAGER_H
#define _MOVEMENTMANAGER_H

class MovementManager
{
    public:
        /// @brief Stops all the motors
        virtual void StopAllMotors() = 0;

        /// @brief Moves the main device to horizontal grid position
        /// @param x x-axis coordinate
        /// @param y y-axis coordinate
        virtual int MoveToXY(float x, float y) = 0;

        /// @brief Moves device to set height
        /// @param z The desired height
        virtual int MoveToHeight(float z) = 0;

        /// @brief Moves to max possible height.
        /// @return the measured height in centimeters
        virtual int MoveToMaxHeight() = 0;

        /// @brief Turns the device on the horizontal plane
        /// @param alpha The desired angle
        virtual int TurnHorizontalPlane(float alpha) = 0;

        /// @brief Turns the device on the vertical plane
        /// @param angle The desired angle
        virtual int TurnVerticalPlane(float beta) = 0;

    protected:
        float _x, _y, _z, _alpha, _beta;
        bool _currentMovementError;
};

#endif
