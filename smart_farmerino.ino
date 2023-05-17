// #define DEBUG

#define REQUEST_SEPARATOR "!"
#define PARAM_SEPARATOR "#"

#define COMMAND_END_PREFIX "END"
#define COMMAND_UPDATE_PREFIX "UPD"

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

float _x, _y, _z, _alpha, _beta;

void setup() {
  Serial.begin(9600);

  _command = NULL;
  _requestId = NULL;
  _parameters = NULL;
  _sleepSeconds = -1;

  _x = _y = _z = _alpha = _beta = 0;

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
  unsigned int length = requestStr.length()+1;
  char* request = new char[length];
  requestStr.toCharArray(request, length);

  // tokenize it to extract information
  _requestId = strtok(request, REQUEST_SEPARATOR);
  _command = strtok(NULL, REQUEST_SEPARATOR);
  _parameters = strtok(NULL, REQUEST_SEPARATOR);
}

short ValidCommand() {
// #ifdef DEBUG
//   // checking command essential values
//   Serial.print("request: ");
//   Serial.println(_requestId);

//   Serial.print("command: ");
//   Serial.println(_command);

//   Serial.print("params: ");
//   Serial.println(_parameters);
// #endif

  return _requestId != NULL && _command != NULL;
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

void ReturnWithError() {
  CleanSerial();
  
  Serial.print(COMMAND_END_PREFIX);
  Serial.print(_requestId);
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(ERROR_GENERIC);
}

void ReturnCommandResult() {
  CleanSerial();

  Serial.print(COMMAND_END_PREFIX);
  Serial.print(_requestId);
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(_commandResult);
}

void ReturnCommandPartialResult(char* value) {
  CleanSerial();

  Serial.print(COMMAND_UPDATE_PREFIX);
  Serial.print(_requestId);
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(value);
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

#ifdef DEBUG
  Serial.print("getting parameters from ");
  Serial.println(_parameters);
#endif

  *par1 = strtok(_parameters, PARAM_SEPARATOR);
  *par2 = strtok(NULL, PARAM_SEPARATOR);

#ifdef DEBUG
  Serial.print("p1: ");
  Serial.print(*par1);
  Serial.print("\np2: ");
  Serial.println(*par2);
#endif

  return *par1 != NULL && *par2 != NULL;
}

/// @brief Stops all the motors
void StopAllMotors() {
#ifdef DEBUG
  Serial.println("Stopping all motors");
#endif

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
#ifdef DEBUG
  char buffer[80];
  sprintf(buffer, "x: %f -> %f\ny: %f -> %f", _x, x, _y, y);
  Serial.println(buffer);
#endif

  char partialPos[50];

  float xCmUpdate = 5; // increment when update
  float yCmUpdate = 10; // increment when update

  float xVel = 0.01 * xCmUpdate; // m/s
  float yVel = 0.01 * yCmUpdate; // m/s

  short xStepFactor = (x > _x) ? 1 : -1;
  short yStepFactor = (y > _y) ? 1 : -1;

  while (_x != x)
  {
    float xStepValue = min(xCmUpdate, abs(x - _x));

    delay(xStepValue * xVel * 1000);

    _x += xStepFactor*xStepValue;

    sprintf(partialPos, "%f%s%f", _x, PARAM_SEPARATOR, _y);
    ReturnCommandPartialResult(partialPos);
  }

  while (_y != y)
  {
    float yStepValue = min(yCmUpdate, abs(y - _y));

    delay(yStepValue * yVel * 1000);

    _y += yStepFactor*yStepValue;

    sprintf(partialPos, "%f%s%f", _x, PARAM_SEPARATOR, _y);
    ReturnCommandPartialResult(partialPos);
  }

  _commandResult = NO_ERROR;
#ifdef DEBUG
  Serial.println("finished");
#endif
}

/// @brief Moves device to set height
/// @param z The desired height
void MoveToHeight(float z) {
#ifdef DEBUG
  char buffer[40];
  sprintf(buffer, "z: %f->%f", _z, z);
  Serial.println(buffer);
#endif

  char partialPos[50];

  float zCmUpdate = 5; // increment when update
  float zVel = 0.01 * zCmUpdate; // m/s
  short zStepFactor = (z > _z) ? 1 : -1;

  while (_z != z)
  {
    float zStepValue = min(zCmUpdate, abs(z - _z));

    delay(zStepValue * zVel * 1000);

    _z += zStepFactor * zStepValue;

    sprintf(partialPos, "%f", _z);
    ReturnCommandPartialResult(partialPos);
  }
}

/// @brief Moves to max possible height.
/// @return the measured height in centimeters
void MoveToMaxHeight() {
#ifdef DEBUG
  char buffer[40];
  sprintf(buffer, "going to max height");
  Serial.println(buffer);
#endif

  delay(4000);
  _z = 15;
  _commandResult = (int)_z;
}

/// @brief Turns the device on the horizontal plane
/// @param alpha The desired angle
void TurnHorizontalPlane(float alpha) {
#ifdef DEBUG
  char buffer[40];
  sprintf(buffer, "going to horizontal angle %f", alpha);
  Serial.println(buffer);
#endif

  delay(1000);
  _alpha = alpha;
}

/// @brief Turns the device on the vertical plane
/// @param angle The desired angle
void TurnVerticalPlane(float beta) {
#ifdef DEBUG
  char buffer[40];
  sprintf(buffer, "going to vertical angle %f", beta);
  Serial.println(buffer);
#endif

  delay(1000);
  _beta = beta;
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

/// @brief Runs all commands after returned value
void ExecPostReturnCommands() {
  if (_sleepSeconds > 0) {
#ifdef DEBUG
    char buffer[40];
    sprintf(buffer, "sleeping %d seconds", _sleepSeconds);
    Serial.println(buffer);
#endif

    delay(_sleepSeconds * 1000);
  }

  ResetCommand();
}