#define REQUEST_SEPARATOR '!'
#define PARAM_SEPARATOR '#'

String _command;
String _commandPrefix;
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

  _command = teststr;

  int lastIndex = teststr.indexOf(REQUEST_SEPARATOR);
  _commandPrefix = teststr.substring(0, lastIndex);
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
  Serial.println(_commandPrefix + REQUEST_SEPARATOR + "-1");
  ResetCommand();
}

void ReturnCommandResult() {
  CleanSerial();
  Serial.println(_commandPrefix + REQUEST_SEPARATOR + _commandResult);
  ResetCommand();
}

void CleanSerial() {
  Serial.flush();
}

void ResetCommand() {
  _command = "";
  _commandPrefix = "";
}
