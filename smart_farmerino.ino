#define REQUEST_SEPARATOR "!"
#define PARAM_SEPARATOR "#"

#define COMMAND_STOP "STP"
#define COMMAND_SLEEP "SLP"
#define COMMAND_MOVE_TO_XY "MXY"
#define COMMAND_MOVE_TO_HEIGHT "MHT"
#define COMMAND_MOVE_TO_MAX_HEIGHT "MMH"
#define COMMAND_TURN_HORIZONTAL "THZ"
#define COMMAND_TURN_VERTICAL "TVT"
#define COMMAND_PUMP "PMP"

#define NO_ERROR 1
#define ERROR_GENERIC -1
#define ERROR_INVALID_PARAMETERS -2
#define ERROR_INVALID_COMMAND -3

char* _command;
char* _requestId;
char* _parameters;
int _commandResult;

int _sleepSeconds;

void setup() {
  Serial.begin(9600);

  _command = NULL;
  _requestId = NULL;
  _parameters = NULL;
  _sleepSeconds = -1;

  _commandResult = NO_ERROR;
}

void loop() {
  // get command
  GetCommand();

  // check command
  if (!ValidCommand()) {
    ReturnWithError();
    return;
  }

  // execute command
  ExecuteCommand();
  // flush and return
  ReturnCommandResult();
  // Eventually exec post-return commands
  ExecPostReturnCommands();
}

void GetCommand() {
  while (Serial.available() == 0) { delay(500); }     //wait for data available

  // getting command from serial
  String requestStr = Serial.readString();
  requestStr.trim(); // remove any \r \n whitespace at the end of the String

  // convert it to char*
  int length = requestStr.length();
  char* request = new char[length];
  requestStr.toCharArray(request, length+1);

  // tokenize it to extract information
  _requestId = strtok(request, REQUEST_SEPARATOR);
  _command = strtok(NULL, REQUEST_SEPARATOR);
  _parameters = strtok(NULL, REQUEST_SEPARATOR);
}

short ValidCommand() {
  // // checking command essential values
  // Serial.print("request: ");
  // Serial.println(_requestId);

  // Serial.print("command: ");
  // Serial.println(_command);

  // Serial.print("params: ");
  // Serial.println(_parameters);

  return _requestId != NULL && _command != NULL;
}

void ExecuteCommand() {
  _commandResult = NO_ERROR;

  // Command stop
  if (!strcmp(_command, COMMAND_STOP)) {
    StopAllMotors();
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

    MoveToXY(x, y);
    return;
  }

  // Moves to height
  if (!strcmp(_command, COMMAND_MOVE_TO_HEIGHT)) {
    float height = atof(_parameters);

    MoveToHeight(height);
    return;
  }
  
  // Moves to max height
  if (!strcmp(_command, COMMAND_MOVE_TO_MAX_HEIGHT)) {
    MoveToMaxHeight();
    return;
  }

  // Turns on horizontal plane
  if (!strcmp(_command, COMMAND_TURN_HORIZONTAL)) {
    float hAngle = atof(_parameters);

    TurnHorizontalPlane(hAngle);
    return;
  }

  // Turns on vertical plane
  if (!strcmp(_command, COMMAND_TURN_VERTICAL)) {
    float vAngle = atof(_parameters);

    TurnVerticalPlane(vAngle);
    return;
  }

  // Turns the Pump
  if (!strcmp(_command, COMMAND_PUMP)) {
    char *pNumberStr = "\0\0\0\0\0\0\0\0\0\0";
    char *pStatus = "\0\0\0\0\0\0\0\0\0\0";

    if (!GetParams(&pNumberStr, &pStatus)) {
      _commandResult = ERROR_INVALID_PARAMETERS;
      return;
    }

    float pNumber = atoi(pNumberStr);

    ManagePump(pNumber, pStatus);
    return;
  }

  _commandResult = ERROR_INVALID_COMMAND;
}

void ReturnWithError() {
  CleanSerial();
  
  Serial.print(_requestId);
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(ERROR_GENERIC);
}

void ReturnCommandResult() {
  CleanSerial();

  Serial.print(_requestId);
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(_commandResult);
}

void CleanSerial() {
  Serial.flush();
}

void ResetCommand() {
  _requestId = NULL;
  _command = NULL;
  _parameters = NULL;

  _sleepSeconds = -1;
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

/// @brief Stops all the motors
void StopAllMotors() {
  //TODO
  // Serial.println("Stopping all motors");
  delay(500);
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

/// @brief Moves the main device to horizontal grid position
/// @param x x-axis coordinate
/// @param y y-axis coordinate
void MoveToXY(float x, float y) {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "going to %f, %f", x, y);
  // Serial.println(buffer);

  delay(3000);
}

/// @brief Moves device to set height
/// @param height The desired height
void MoveToHeight(float height) {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "going to height %f", height);
  // Serial.println(buffer);

  delay(3000);
}

/// @brief Moves to max possible height.
/// @return the measured height in centimeters
void MoveToMaxHeight() {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "going to max height");
  // Serial.println(buffer);

  delay(4000);
  _commandResult = 15;
}

/// @brief Turns the device on the horizontal plane
/// @param angle The desired angle
void TurnHorizontalPlane(float angle) {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "going to horizontal angle %f", angle);
  // Serial.println(buffer);

  delay(1000);
}

/// @brief Turns the device on the vertical plane
/// @param angle The desired angle
void TurnVerticalPlane(float angle) {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "going to vertical angle %f", angle);
  // Serial.println(buffer);

  delay(1000);
}

/// @brief Manages the pumps. Each pump is identified by a number. Status admitted: "on", "off"
/// @param pumpNumber the given pump number
/// @param pumpStatus the desired pump status
void ManagePump(int pumpNumber, char* pumpStatus) {
  //TODO
  // char buffer[40];
  // sprintf(buffer, "sets pump %d status %s", pumpNumber, pumpStatus);
  // Serial.println(buffer);

  delay(500);
}

/// @brief Runs all commands after returned value
void ExecPostReturnCommands() {
  if (_sleepSeconds > 0) {
    // char buffer[40];
    // sprintf(buffer, "sleeping %d seconds", _sleepSeconds);
    // Serial.println(buffer);

    delay(_sleepSeconds * 1000);
  }

  ResetCommand();
}