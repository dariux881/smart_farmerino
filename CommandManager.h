#ifndef _COMMANDMANAGER_H
#define _COMMANDMANAGER_H

#define COMMAND_STOP "STP"
#define COMMAND_SLEEP "SLP"
#define COMMAND_MOVE_TO_XY "MXY"
#define COMMAND_MOVE_TO_HEIGHT "MHT"
#define COMMAND_MOVE_TO_MAX_HEIGHT "MMH"
#define COMMAND_TURN_HORIZONTAL "THZ"
#define COMMAND_TURN_VERTICAL "TVT"
#define COMMAND_PUMP "PMP"

#define ERROR_INVALID_PARAMETERS -2
#define ERROR_INVALID_COMMAND -3

#include "MovementManager.h"

class CommandManager
{
    
  public:
    CommandManager(MovementManager* movementManager)
    {
        _movementManager = movementManager;
        _command = _requestId = _parameters = NULL;
        _commandResult = NO_ERROR;
        _sleepSeconds = -1;
        _commandValid = 0;
    }

    char* GetCommand() { return _command; }
    char* GetRequestId() { return _requestId; }
    int GetCommandResult() { return _commandResult; }

    short ParseCommand(String requestStr)
    {
        requestStr.trim(); // remove any \r \n whitespace at the end of the String

        // convert it to char*
        unsigned int length = requestStr.length()+1;
        char* request = new char[length];
        requestStr.toCharArray(request, length);

        // tokenize it to extract information
        _requestId = strtok(request, REQUEST_SEPARATOR);
        _command = strtok(NULL, REQUEST_SEPARATOR);
        _parameters = strtok(NULL, REQUEST_SEPARATOR);

        _commandValid = ValidCommand();
        return _commandValid;
    }

    short IsCommandValid()
    {
        return _commandValid;
    }

    void ExecuteCommand() {
        _commandResult = NO_ERROR;

        #ifdef DEBUG
        
        // checking command essential values
        Serial.print("exec: ");
        Serial.println(_command);

        #endif

        // Command stop
        if (!strcmp(_command, COMMAND_STOP)) {
            _movementManager->StopAllMotors();
            _commandResult = NO_ERROR;
            return;
        }

        // Command sleep
        if (!strcmp(_command, COMMAND_SLEEP)) {
            int sleepSeconds = atoi(_parameters);

            PrepareSleepCommand(sleepSeconds);
            return;
        }

        // Command move to xy
        if (!strcmp(_command, COMMAND_MOVE_TO_XY)) {
            char *xstr = "\0\0\0\0\0\0\0\0\0\0";
            char *ystr = "\0\0\0\0\0\0\0\0\0\0";

            if (!GetParams(&xstr, &ystr)) {
                _commandResult = ERROR_INVALID_PARAMETERS;
                return;
            }

            float x = atof(xstr);
            float y = atof(ystr);

            _commandResult = _movementManager->MoveToXY(x, y);
            return;
        }

        // Moves to height
        if (!strcmp(_command, COMMAND_MOVE_TO_HEIGHT)) {
            float height = atof(_parameters);

            _commandResult = _movementManager->MoveToHeight(height);
            return;
        }
        
        // Moves to max height
        if (!strcmp(_command, COMMAND_MOVE_TO_MAX_HEIGHT)) {
            _commandResult = _movementManager->MoveToMaxHeight();
            return;
        }

        // Turns on horizontal plane
        if (!strcmp(_command, COMMAND_TURN_HORIZONTAL)) {
            float hAngle = atof(_parameters);

            _commandResult = _movementManager->TurnHorizontalPlane(hAngle);
            return;
        }

        // Turns on vertical plane
        if (!strcmp(_command, COMMAND_TURN_VERTICAL)) {
            float vAngle = atof(_parameters);

            _commandResult = _movementManager->TurnVerticalPlane(vAngle);
            return;
        }

        // Turns the Pump
        if (!strcmp(_command, COMMAND_PUMP)) {
            char *pNumberStr = "\0\0\0\0\0\0\0\0\0\0";
            char *pAmountStr = "\0\0\0\0\0\0\0\0\0\0";

            if (!GetParams(&pNumberStr, &pAmountStr)) {
                _commandResult = ERROR_INVALID_PARAMETERS;
                return;
            }

            short pNumber = atoi(pNumberStr);
            float pAmount = atof(pAmountStr);

            ManagePump(pNumber, pAmount);
            return;
        }

        _commandResult = ERROR_INVALID_COMMAND;
    }

    void ExecPostReturnCommand()
    {
        if (_sleepSeconds > 0) {
            #ifdef DEBUG

            char buffer[40];
            sprintf(buffer, "sleeping %d seconds", _sleepSeconds);
            Serial.println(buffer);

            #endif

            delay(_sleepSeconds * 1000);

            #ifdef DEBUG

            Serial.println("ready again");

            #endif
}
    }

    void ResetCommand() {
        _requestId = NULL;
        _command = NULL;
        _parameters = NULL;
        _commandValid = 0;

        _sleepSeconds = -1;
    }

  private:
    MovementManager* _movementManager;
    char* _command;
    char* _requestId;
    char* _parameters;
    int _commandResult;
    int _sleepSeconds;
    short _commandValid;
    
    short ValidCommand()
    {
        return _requestId != NULL && _command != NULL;
    }

    /// @brief Processes _parameters to get 2 parameters
    /// @param par1 first expected parameter
    /// @param par2 second expected parameter
    /// @return 0 in case of errors, 1 if all is ok
    int GetParams(char **par1, char **par2) {
        if (_parameters == NULL || *par1 == NULL || *par2 == NULL) return 0;

        *par1 = strtok(_parameters, PARAM_SEPARATOR);
        *par2 = strtok(NULL, PARAM_SEPARATOR);

        return *par1 != NULL && *par2 != NULL;
    }

    /// @brief Sets sleep seconds internally. Does not immediately sleeps
    /// @param sleepSeconds seconds to sleep
    void PrepareSleepCommand(int sleepSeconds) {
        if (sleepSeconds > 0) {
            _sleepSeconds = sleepSeconds;
        }
        else {
            _sleepSeconds = -1;
        }
    }

    /// @brief Manages the pumps. Each pump is identified by a number.
    /// @param pumpNumber the given pump number
    /// @param amountInLiters the desired water amount in liters
    void ManagePump(int pumpNumber, float amountInLiters) {
        #ifdef DEBUG

        char buffer[40];
        sprintf(buffer, "sets pump %d to provide %f L", pumpNumber, amountInLiters);
        Serial.println(buffer);
        
        #endif

        delay(500);
    }

};  // CommandManager class

#endif
