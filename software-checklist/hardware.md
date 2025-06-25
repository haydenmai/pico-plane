# Hardware Abstraction Layer Checklist
There are many factors that may contribute to a piece of hardware failing. This includes but not limited to: overvoltage, unsafe code, harsh environment, and defects. 

One factor we can attempt to remedy is unsafe code via robust handling and hard-limit input values, as well as ensuring hardware does not behave outside of its intended behaviour.

## Watchdog
- If control link is lost for 1 second, disarm.
- Send arming signal to dji air unit to get full vtx power
- on screen display (battery voltage, link quality, rssi, etc)

## Electronic Speed Control
- [ ] Was it correctly programmed? (50hz PWM signals, refer to source)
- [ ] Is there a value limit checker? If value is over/under how is it handled?
- [ ] Are the values mapped to the ESC correct?
- [ ] If a throttle limit is enabled, is it properly used/set?
- [ ] Is there a secondary check to disable the throttle when unarmed? 

## MPU6050

## Servo: DS-M005
