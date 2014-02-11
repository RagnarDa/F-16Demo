--------------------------------------------------------------------------
--------------------------------------------------------------------------
Release 0.2 (12/21/2012)
--------------------------------------------------------------------------
--------------------------------------------------------------------------
- Initial source code release

- TODO: Reference source documenations within code!

--------------------------------------------------------------------------
--------------------------------------------------------------------------
Release 0.1 (12/20/2012)
--------------------------------------------------------------------------
--------------------------------------------------------------------------
-This release is the initial closed source version with the open source version coming
 shortly
-This release is a "for fun" release to the community to play around with the current
 state of the F-16

IMPORTANT!!!
-If you are on an x86 (Win32) system make sure you swap the DLL in the bin directory
 with the dll in the bin/x86 directory
-Due to the lack of ground handling from limitations in the publicly available EFM
 API you should start this aircraft in the air...you can take off but it is like
 a skating rink!
-Don't add any weapons...see the videos in my F-16 thread

To get this running:
-Place the F-16A Folder in to your DCS World/Mods/aircrafts directory

See below for flight model code notes:

********************************************************
 THIS IS JUST A COPY/PASTE FROM THE FM HEADER
********************************************************
//--------------------------------------------------------------------------
// F-16 Demo External Flight Model for DCS World
// 
// Author: CptSmiley (forums.eagle.ru username)
//
// Use Only for Non-Commercial Purposes
//
//--------------------------------------------------------------------------
// Source Data:
// 1) F-16 University of Minnesota Non-Linear Flight Model
//	  http://www.aem.umn.edu/people/faculty/balas/darpa_sec/SEC.Software.html
// 2) NASA TP 1538 Simulator Study of Stall/Post-Stall Characteristics of a 
//	  Fighter Airplane With Relaxed Longitudinal Static Stability
// 3) NASA TN D-8176 Simulator Study of the Effectiveness of an Automatic Control
//    System Designed to Improve the High Angle-of-Attack Characteristics of a
//    Fighter Airplane
// 4) AFIT/GE/ENG/88D-8 A Real-time Simulator for Man-In-The-Loop Testing of
//    Aircraft Control Systems
// 5) JBSim 1.0 F-16 Configuration 
// 6) F-16A and F-16C Flight Manual
//
//--------------------------------------------------------------------------
// F-16Demo.cpp : Defines the exported functions for the DLL application.
// Control the main portion of the discrete simulation event
//
// This project will compile a DLL.  This DLL needs to be compiled with the
// same machine type of your machine (x86 or x64).  This DLL then needs to
// be placed within the bin directory in your mod/aircraft/XXXairplane/ 
// directory within DCS World.  
//
// See associated entry.lua for how to tell the mod to use the DLL flight
// model
//--------------------------------------------------------------------------
// IMPORTANT!  COORDINATE CONVENTION:
//
// DCS WORLD Convention:
// Xbody: Out the front of the nose
// Ybody: Out the top of the aircraft
// Zbody: Out the right wing
//
// Normal Aerodynamics/Control Convention:
// Xbody: Out the front of the nose
// Ybody: Out the right wing
// Zbody: Out the bottom of the aircraft
//
// This means that if you are referincing from any aerodynamic, stabilty, or control document
// they are probably using the second set of directions.  Which means you always need to switch
// the Y and the Z and reverse the Y prior to output to DCS World
//---------------------------------------------------------------------------
// TODO List:
// -Make code more "object-oriented"...
// -Differential command into the pitch controller
// -Weight on wheels determination
// -Ground reaction modeling
// -Fix actuator dynamics
// -Improve look-up tables
// -Speed brake effects and control
//---------------------------------------------------------------------------
// KNOWN Issues:
// -On ground, the FCS controls flutter due to no filtering of alpha and Nz.
//  Need logic to determine when on ground (hackish right now) to zero those
//  signals out.
// -Aircraft naturally trims to 1.3g for some reason, need to apply -0.3 pitch
//  trim to get aircraft to trim at 1.0g for flight controller
// -Actuators cause flutter at high speed due to filtering of sensor signals
//  Removed servo-dynamics until I can figure this out
// -Gear reaction happening but ground handling not modeled due to lack of available
//  API calls
// -Gear automatically drops at 200ft to allow simple touch downs
//---------------------------------------------------------------------------