#define REQUEST_SEPARATOR '!'
#define PARAM_SEPARATOR '#'

char* _command;
char* _requestId;
char* _parameters;
int _commandResult;

void setup() {
  Serial.begin(9600);
  //_command = "";
  _commandResult = 0;
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
}

void GetCommand() {
  while (Serial.available() == 0) { delay(500); }     //wait for data available

  String teststr = Serial.readString();
  teststr.trim(); // remove any \r \n whitespace at the end of the String

  // getting request ID
  int reqIdSeparatorIndex = teststr.indexOf(REQUEST_SEPARATOR);
  _requestId = new char[reqIdSeparatorIndex];
  teststr
    .toCharArray(_requestId, reqIdSeparatorIndex, 0);

  // getting command ID
  int commandSeparatorIndex = teststr.lastIndexOf(REQUEST_SEPARATOR);
  unsigned short commandLength = commandSeparatorIndex - reqIdSeparatorIndex + 1;
  _command = new char[commandLength];
  teststr
    .toCharArray(_command, commandLength, reqIdSeparatorIndex);

  // getting parameters
  unsigned short parametersLength = teststr.length() - commandSeparatorIndex + 1;
  if (parametersLength > 0)
  {
    _parameters = new char[parametersLength];
    teststr
      .toCharArray(_parameters, parametersLength, commandSeparatorIndex);    
  }
  else
  {
    _parameters = NULL;
  }
}

short ValidCommand() {
  //TODO check command
  return 1;
}

void ExecuteCommand() {
  //TODO exec _command

  delay(random(1, 10)*1000);
  
  //TODO set _commandResult
  _commandResult = 1;
}

void ReturnWithError() {
  CleanSerial();
  
  Serial.print(_requestId + REQUEST_SEPARATOR);
  Serial.println("-1");

  ResetCommand();
}

void ReturnCommandResult() {
  CleanSerial();

  Serial.print(_requestId + REQUEST_SEPARATOR);
  Serial.println(_commandResult);

  ResetCommand();
}

void CleanSerial() {
  Serial.flush();
}

void ResetCommand() {
  _requestId = NULL;
  _command = NULL;
  _parameters = NULL;
}
