#include <ArduinoBLE.h>

#define DEBUG

#define REQUEST_SEPARATOR "!"
#define PARAM_SEPARATOR "#"

#define COMMAND_END_PREFIX "END"
#define COMMAND_UPDATE_PREFIX "UPD"

#define NO_ERROR 1
#define ERROR_GENERIC -1

#include "CommandManager.h"
#include "MovementManager.h"

CommandManager* _commandManager;
MovementManager* _movementManager;

void setup() {
  Serial.begin(9600);

  _movementManager = new MovementManager(ReturnCommandPartialResult);
  _commandManager = new CommandManager(_movementManager);
}

void loop() {
  // get command
  short commandPresent = GetCommand();

  if (!commandPresent)
  {
    return;
  }

  // check command
  if (!_commandManager->IsCommandValid()) {
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

short GetCommand() {
  while (Serial.available() == 0) { delay(500); }     //wait for data available

  // getting command from serial
  return _commandManager->ParseCommand(Serial.readString());
}

void ExecuteCommand()
{
  _commandManager->ExecuteCommand();
}

void ReturnWithError() {
  CleanSerial();
  
  Serial.print(COMMAND_END_PREFIX);
  Serial.print(_commandManager->GetRequestId());
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(ERROR_GENERIC);
}

void ReturnCommandResult() {
  CleanSerial();

  Serial.print(COMMAND_END_PREFIX);
  Serial.print(_commandManager->GetRequestId());
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(_commandManager->GetCommandResult());
}

void ReturnCommandPartialResult(char* value) {
  CleanSerial();

  Serial.print(COMMAND_UPDATE_PREFIX);
  Serial.print(_commandManager->GetRequestId());
  Serial.print(REQUEST_SEPARATOR);
  Serial.println(value);
}

void CleanSerial() {
  Serial.flush();
}

void ResetCommand() {
  _commandManager->ResetCommand();
}

/// @brief Runs all commands after returned value
void ExecPostReturnCommands() {
  _commandManager->ExecPostReturnCommand();

  ResetCommand();
}
