local res = external_profile("Config/Input/Aircrafts/base_joystick_binding.lua")
join(res.keyCommands,{

-- Autopilot
{down = iCommandPlaneAutopilot, name = 'Autopilot', category = 'Autopilot'},
{down = iCommandPlaneSAUHBarometric, name = 'Autopilot - Barometric Altitude Hold \'H\'', category = 'Autopilot'},
{down = iCommandPlaneAutopilotOverrideOn, up = iCommandPlaneAutopilotOverrideOff, name = 'Autopilot override', category = 'Autopilot'},
{down = iCommandPlaneStabTangBank, name = 'Autopilot - Attitude Hold', category = 'Autopilot'},
{down = iCommandPlaneStabHbarBank, name = 'Autopilot - Altitude And Roll Hold', category = 'Autopilot'},
{down = iCommandPlaneStabHorizon,	name = 'Autopilot - Transition To Level Flight Control', category = 'Autopilot'},
{down = iCommandPlaneStabHbar, name = 'Autopilot - Barometric Altitude Hold', category = 'Autopilot'},
{down = iCommandPlaneStabHrad, name = 'Autopilot - Radar Altitude Hold', category = 'Autopilot'},
{down = iCommandPlaneRouteAutopilot, name = 'Autopilot - \'Route following\'', category = 'Autopilot'},
{down = iCommandPlaneStabCancel, name = 'Autopilot Disengage', category = 'Autopilot'},

-- Systems
{down = iCommandPlaneFuelOn, up = iCommandPlaneFuelOff, name = 'Fuel Dump', category = 'Systems'},
{down = iCommandPlaneJettisonFuelTanks, name = 'Jettison Fuel Tanks', category = 'Systems'},
{down = iCommandPowerOnOff, name = 'Electric Power Switch', category = 'Systems'},
{pressed = iCommandAltimeterPressureIncrease,	up = iCommandAltimeterPressureStop, name = 'Altimeter Pressure Increase Su-25T', category = 'Systems'},
{pressed = iCommandAltimeterPressureDecrease, up = iCommandAltimeterPressureStop, name = 'Altimeter Pressure Decrease Su-25T', category = 'Systems'},

-- Modes
{down = iCommandPlaneModeFI0, name = '(6) Longitudinal Missile Aiming Mode', category = 'Modes'},
{down = iCommandPlaneModeGround, name = '(7) Air-To-Ground Mode', category = 'Modes'},
{down = iCommandPlaneModeGrid, name = '(8) Gunsight Reticle Switch', category = 'Modes'},

-- Sensors
{down = iCommandPlaneChangeLock, name = 'Target Lock', category = 'Sensors'},
{down = iCommandRefusalTWS, name = 'Target Unlock', category = 'Sensors'},
{down = iCommandPlaneRadarOnOff, name = 'ELINT Pod On/Off', category = 'Sensors'},
{down = iCommandPlaneRadarCenter, name = 'Target Designator To Center', category = 'Sensors'},
{down = iCommandPlaneEOSOnOff, name = 'Electro-Optical System On/Off', category = 'Sensors'},
{down = iCommandPlaneLaserRangerOnOff, name = 'Laser Ranger On/Off', category = 'Sensors'},
{down = iCommandPlaneNightTVOnOff, name = 'Night Vision (FLIR or LLTV) On/Off', category = 'Sensors'},
{pressed = iCommandPlaneRadarUp, up = iCommandPlaneRadarStop, name = 'Target Designator Up', category = 'Sensors'},
{pressed = iCommandPlaneRadarDown, up = iCommandPlaneRadarStop, name = 'Target Designator Down', category = 'Sensors'},
{pressed = iCommandPlaneRadarLeft, up = iCommandPlaneRadarStop, name = 'Target Designator Left', category = 'Sensors'},
{pressed = iCommandPlaneRadarRight, up = iCommandPlaneRadarStop, name = 'Target Designator Right', category = 'Sensors'},

{down = iCommandPlaneZoomIn, name = 'Display Zoom In', category = 'Sensors'},
{down = iCommandPlaneZoomOut, name = 'Display Zoom Out', category = 'Sensors'},
{down = iCommandDecreaseRadarScanArea, name = 'Target Size / Gunpod Elevation Decrease Stepping', category = 'Sensors'},
{down = iCommandIncreaseRadarScanArea, name = 'Target Size / Gunpod Elevation Increase Stepping', category = 'Sensors'},
{pressed = iCommandPlaneIncreaseBase_Distance, up = iCommandPlaneStopBase_Distance, name = 'Target Size / Gunpod Elevation Increase', category = 'Sensors'},
{pressed = iCommandPlaneDecreaseBase_Distance, up = iCommandPlaneStopBase_Distance, name = 'Target Size / Gunpod Elevation Decrease', category = 'Sensors'},
{down = iCommandChangeRWRMode, name = 'RWR/SPO Mode Select', category = 'Sensors'},
{down = iCommandPlaneThreatWarnSoundVolumeDown, name = 'RWR/SPO Sound Signals Volume Down', category = 'Sensors'},
{down = iCommandPlaneThreatWarnSoundVolumeUp, name = 'RWR/SPO Sound Signals Volume Up', category = 'Sensors'},

-- Weapons                                                                        
{down = iCommandPlaneSalvoOnOff, name = 'Salvo Mode', category = 'Weapons'},
--{combos = {{key = 'Space', reformers = {'RAlt'}}}, down = iCommandPlanePickleOn,	up = iCommandPlanePickleOff, name = 'Weapon Release', category = 'Weapons'},
{down = iCommandChangeGunRateOfFire, name = 'Cut Of Burst select', category = 'Weapons'},
{down = iCommandChangeRippleQuantity, name = 'Ripple Quantity Select/SPPU select', category = 'Weapons'},
{down = iCommandChangeRippleInterval, name = 'Ripple Interval Increase', category = 'Weapons'},
{down = iCommandChangeRippleIntervalDown, name = 'Ripple Interval Decrease', category = 'Weapons'},

-- Countermeasures
{down = iCommandActiveIRJamming, name = 'IR Jamming', category = 'Countermeasures'},
})
-- joystick axes 
join(res.axisCommands,{
{action = iCommandPlaneSelecterHorizontalAbs, name = 'I-251 Slew Horizontal'},
{action = iCommandPlaneSelecterVerticalAbs	, name = 'I-251 Slew Vertical'},
{action = iCommandPlaneMFDZoomAbs 			, name = 'I-251 Zoom'},
{action = iCommandPlaneBase_DistanceAbs 	, name = 'Target Box Size / Funnel target wing span / Gunpod Elevation'},
})
return res