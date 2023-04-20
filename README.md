# smart_farmerino
Smart Farmer - Arduino side

# Supported commands
- *STP* Stops all current execution. Resets current operation as well
- *SLP* Sleeps of the given amount of time
- *MXY* Moves to position XY
- *MHT* Moves to height
- *MMH* Moves to max height
- *THZ* Turns on horizontal plane
- *TVT* Turns on vertical plane
- *PMP* Turns the Pump on/off. Two parameters expected: number of pump, status on/off

# Errors
The following errors can be raised:
- INVALID_PARAMETERS -1