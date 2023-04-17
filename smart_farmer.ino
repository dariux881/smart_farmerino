String _command;
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
  while (Serial.available() == 0) {}     //wait for data available

  String teststr = Serial.readStringUntil('!');
  teststr.trim(); // remove any \r \n whitespace at the end of the String

  _command = teststr;
}

short ValidCommand() {
  //TODO check command
  return 1;
}

void ExecuteCommand() {
  //TODO exec _command
  delay(1000);
  
  //TODO set _commandResult
  _commandResult = 1;
  Serial.println(_command);
}

void ReturnWithError() {
  CleanSerial();
  Serial.println(-1);
  ResetCommand();
}

void ReturnCommandResult() {
  CleanSerial();
  Serial.println(_commandResult);
  ResetCommand();
}

void CleanSerial() {
  Serial.flush();
}

void ResetCommand() {
  _command = "";
}
