local res = external_profile("Config/Input/Aircrafts/base_keyboard_binding.lua")
join(res.keyCommands,{

-- Autopilot
{combos = {{key = 'A'}}, down = iCommandPlaneAutopilot, name = 'Autopilot', category = 'Autopilot'},
{combos = {{key = 'H'}}, down = iCommandPlaneSAUHBarometric, name = 'Autopilot - Barometric Altitude Hold \'H\'', category = 'Autopilot'},
{combos = {{key = '`', reformers = {'LAlt'}}}, down = iCommandPlaneAutopilotOverrideOn, up = iCommandPlaneAutopilotOverrideOff, name = 'Autopilot override (Su-25T)', category = 'Autopilot'},
{combos = {{key = '1', reformers = {'LAlt'}}}, down = iCommandPlaneStabTangBank, name = 'Autopilot - Attitude Hold', category = 'Autopilot'},
{combos = {{key = '2', reformers = {'LAlt'}}}, down = iCommandPlaneStabHbarBank, name = 'Autopilot - Altitude And Roll Hold', category = 'Autopilot'},
{combos = {{key = '3', reformers = {'LAlt'}}}, down = iCommandPlaneStabHorizon,	name = 'Autopilot - Transition To Level Flight Control', category = 'Autopilot'},
{combos = {{key = '4', reformers = {'LAlt'}}}, down = iCommandPlaneStabHbar, name = 'Autopilot - Barometric Altitude Hold', category = 'Autopilot'},
{combos = {{key = '5', reformers = {'LAlt'}}}, down = iCommandPlaneStabHrad, name = 'Autopilot - Radar Altitude Hold', category = 'Autopilot'},
{combos = {{key = '6', reformers = {'LAlt'}}}, down = iCommandPlaneRouteAutopilot, name = 'Autopilot - \'Route following\'', category = 'Autopilot'},
{combos = {{key = '9', reformers = {'LAlt'}}}, down = iCommandPlaneStabCancel, name = 'Autopilot Disengage', category = 'Autopilot'},

-- Systems
{combos = {{key = 'R'}}, down = iCommandPlaneFuelOn, up = iCommandPlaneFuelOff, name = 'Fuel Dump', category = 'Systems'},
--{combos = {{key = 'R', reformers = {'LCtrl'}}}, down = iCommandPlaneAirRefuel, name = 'Refueling Boom', category = 'Systems'},
{combos = {{key = 'R', reformers = {'LAlt'}}}, down = iCommandPlaneJettisonFuelTanks, name = 'Jettison Fuel Tanks', category = 'Systems'},
{combos = {{key = 'L', reformers = {'RShift'}}}, down = iCommandPowerOnOff, name = 'Electric Power Switch', category = 'Systems'},
{combos = {{key = '=', reformers = {'RShift'}}}, pressed = iCommandAltimeterPressureIncrease,	up = iCommandAltimeterPressureStop, name = 'Altimeter Pressure Increase Su-25T', category = 'Systems'},
{combos = {{key = '-', reformers = {'RShift'}}}, pressed = iCommandAltimeterPressureDecrease, up = iCommandAltimeterPressureStop, name = 'Altimeter Pressure Decrease Su-25T', category = 'Systems'},

-- Modes
--{combos = {{key = '2'}}, down = iCommandPlaneModeBVR, name = '(2) Beyond Visual Range Mode', category = 'Modes'},
--{combos = {{key = '3'}}, down = iCommandPlaneModeVS, name = '(3) Close Air Combat Vertical Scan Mode', category = 'Modes'},
--{combos = {{key = '4'}}, down = iCommandPlaneModeBore, name = '(4) Close Air Combat Bore Mode', category = 'Modes'},
--{combos = {{key = '5'}}, down = iCommandPlaneModeHelmet, name = '(5) Close Air Combat HMD Helmet Mode', category = 'Modes'},
{combos = {{key = '6'}}, down = iCommandPlaneModeFI0, name = '(6) Longitudinal Missile Aiming Mode', category = 'Modes'},
{combos = {{key = '7'}}, down = iCommandPlaneModeGround, name = '(7) Air-To-Ground Mode', category = 'Modes'},
{combos = {{key = '8'}}, down = iCommandPlaneModeGrid, name = '(8) Gunsight Reticle Switch', category = 'Modes'},

-- Sensors
{combos = {{key = 'Enter'}}, down = iCommandPlaneChangeLock, name = 'Target Lock', category = 'Sensors'},
{combos = {{key = 'Back'}}, down = iCommandRefusalTWS, name = 'Target Unlock', category = 'Sensors'},
{combos = {{key = 'I'}}, down = iCommandPlaneRadarOnOff, name = 'ELINT Pod On/Off', category = 'Sensors'},
--{combos = {{key = 'I', reformers = {'RAlt'}}}, down = iCommandPlaneRadarChangeMode, name = 'Radar RWS/TWS Mode Select', category = 'Sensors'},
{combos = {{key = 'I', reformers = {'RCtrl'}}}, down = iCommandPlaneRadarCenter, name = 'Target Designator To Center', category = 'Sensors'},
--{combos = {{key = 'I', reformers = {'RShift'}}}, down = iCommandPlaneChangeRadarPRF, name = 'Radar Pulse Repeat Frequency Select', category = 'Sensors'},
{combos = {{key = 'O'}}, down = iCommandPlaneEOSOnOff, name = 'Electro-Optical System On/Off', category = 'Sensors'},
{combos = {{key = 'O', reformers = {'RShift'}}}, down = iCommandPlaneLaserRangerOnOff, name = 'Laser Ranger On/Off', category = 'Sensors'},
{combos = {{key = 'O', reformers = {'RCtrl'}}}, down = iCommandPlaneNightTVOnOff, name = 'Night Vision (FLIR or LLTV) On/Off', category = 'Sensors'},
{combos = {{key = ';'}}, pressed = iCommandPlaneRadarUp, up = iCommandPlaneRadarStop, name = 'Target Designator Up', category = 'Sensors'},
{combos = {{key = '.'}}, pressed = iCommandPlaneRadarDown, up = iCommandPlaneRadarStop, name = 'Target Designator Down', category = 'Sensors'},
{combos = {{key = ','}}, pressed = iCommandPlaneRadarLeft, up = iCommandPlaneRadarStop, name = 'Target Designator Left', category = 'Sensors'},
{combos = {{key = '/'}}, pressed = iCommandPlaneRadarRight, up = iCommandPlaneRadarStop, name = 'Target Designator Right', category = 'Sensors'},
--{combos = {{key = ';', reformers = {'RShift'}}}, pressed = iCommandSelecterUp, up = iCommandSelecterStop, name = 'Scan Zone Up', category = 'Sensors'},
--{combos = {{key = '.', reformers = {'RShift'}}}, pressed = iCommandSelecterDown, up = iCommandSelecterStop, name = 'Scan Zone Down', category = 'Sensors'},
--{combos = {{key = ',', reformers = {'RShift'}}}, pressed = iCommandSelecterLeft, up = iCommandSelecterStop, name = 'Scan Zone Left', category = 'Sensors'},
--{combos = {{key = '/', reformers = {'RShift'}}}, pressed = iCommandSelecterRight, up = iCommandSelecterStop, name = 'Scan Zone Right', category = 'Sensors'},
{combos = {{key = '='}}, down = iCommandPlaneZoomIn, name = 'Display Zoom In', category = 'Sensors'},
{combos = {{key = '-'}}, down = iCommandPlaneZoomOut, name = 'Display Zoom Out', category = 'Sensors'},
{combos = {{key = ']'}}, down = iCommandDecreaseRadarScanArea, name = 'Target Size / Gunpod Elevation Decrease Stepping', category = 'Sensors'},
{combos = {{key = '['}}, down = iCommandIncreaseRadarScanArea, name = 'Target Size / Gunpod Elevation Increase Stepping', category = 'Sensors'},
{combos = {{key = ']', reformers = {'RCtrl'}}}, pressed = iCommandPlaneIncreaseBase_Distance, up = iCommandPlaneStopBase_Distance, name = 'Target Size / Gunpod Elevation Increase', category = 'Sensors'},
{combos = {{key = '[', reformers = {'RCtrl'}}}, pressed = iCommandPlaneDecreaseBase_Distance, up = iCommandPlaneStopBase_Distance, name = 'Target Size / Gunpod Elevation Decrease', category = 'Sensors'},
{combos = {{key = 'R', reformers = {'RShift'}}}, down = iCommandChangeRWRMode, name = 'RWR/SPO Mode Select', category = 'Sensors'},
{combos = {{key = ',', reformers = {'RAlt'}}}, down = iCommandPlaneThreatWarnSoundVolumeDown, name = 'RWR/SPO Sound Signals Volume Down', category = 'Sensors'},
{combos = {{key = '.', reformers = {'RAlt'}}}, down = iCommandPlaneThreatWarnSoundVolumeUp, name = 'RWR/SPO Sound Signals Volume Up', category = 'Sensors'},

-- Weapons                                                                        
{combos = {{key = 'V', reformers = {'LCtrl'}}}, down = iCommandPlaneSalvoOnOff, name = 'Salvo Mode', category = 'Weapons'},
--{combos = {{key = 'Space', reformers = {'RAlt'}}}, down = iCommandPlanePickleOn,	up = iCommandPlanePickleOff, name = 'Weapon Release', category = 'Weapons'},
{combos = {{key = 'C', reformers = {'LShift'}}}, down = iCommandChangeGunRateOfFire, name = 'Cut Of Burst select', category = 'Weapons'},
{combos = {{key = 'Space', reformers = {'LCtrl'}}}, down = iCommandChangeRippleQuantity, name = 'Ripple Quantity Select/SPPU select', category = 'Weapons'},
{combos = {{key = 'V'}}, down = iCommandChangeRippleInterval, name = 'Ripple Interval Increase', category = 'Weapons'},
{combos = {{key = 'V', reformers = {'LShift'}}}, down = iCommandChangeRippleIntervalDown, name = 'Ripple Interval Decrease', category = 'Weapons'},

-- Countermeasures
{combos = {{key = 'E', reformers = {'LShift'}}}, down = iCommandActiveIRJamming, name = 'IR Jamming', category = 'Countermeasures'},
})
return res
