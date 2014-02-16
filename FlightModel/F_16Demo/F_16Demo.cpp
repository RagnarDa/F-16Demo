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
#include "stdafx.h"
#include "F_16Demo.h"
#include "ED_FM_Utility.h"		// Provided utility functions that were in the initial EFM example
#include <Math.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include "UtilityFunctions.h"	// Utility help functions 

// Model headers
#include "Actuators/F16Actuators.h"				//Actuators model functions
#include "Atmosphere/F16Atmosphere.h"			//Atmosphere model functions
#include "Aerodynamics/F16Aero.h"				//Aerodynamic model functions
#include "FlightControls/F16FlightControls.h"	//Flight Controls model functions
#include "Engine/F16Engine.h"					//Engine model functions

//-----------------------------------------------------------------
// This variable is very important.  Make sure you set this
// to 0 at the beginning of each frame time or else the moments
// will accumulate.  For each frame time, add up all the moments
// acting on the air vehicle with this variable using th
//
// Units = Newton * meter
//-----------------------------------------------------------------
Vec3	common_moment;							
//-----------------------------------------------------------------
// This variable is also very important.  This defines all the forces
// acting on the air vehicle.  This also needs to be reset to 0 at the
// beginning of each frame time.  
//
// Units = Newton
//-----------------------------------------------------------------
Vec3	common_force;
//-----------------------------------------------------------------
// Center of gravity of the air vehicle as calculated from the 
// DCS simulation, I don't believe this is utilized within this 
// EFM.
//
// Units = meter
//-----------------------------------------------------------------
Vec3    center_of_gravity;
//-----------------------------------------------------------------
// The moments of inertia for the air vehicle as calculated from the
// DCS Simulation.  This is not used within this EFM as there is a bug
// when trying to manipulate weight or moment of inertia from within
// the EFM.  The inertia is currently set from entry.lua
//
// Units: Newton * meter^2
//-----------------------------------------------------------------
Vec3	inertia;
//-----------------------------------------------------------------
// The local winds acting on the air vehicle as calculated by the
// DCS Simulation
//
// Units: Meters/(Second^2)
//-----------------------------------------------------------------
Vec3	wind;
//-----------------------------------------------------------------
// Absolute velocities of the air vehicle as calculated by DCS World
//
// Units: Meters/(Second^2)
//-----------------------------------------------------------------
Vec3	velocity_world_cs;
//-----------------------------------------------------------------
// Internal fuel as calculated by the EFM fuel system modeling
// (taken out and not used in the F-16)
//-----------------------------------------------------------------
double  internal_fuel;

//-------------------------------------------------------
// Start of F-16 Simulation Variables
// Probably doesn't need it's own namespace or anything
// I just quickly did this to organize my F-16 specific
// variables, needs to be done better eventually
//-------------------------------------------------------
namespace F16
{
	double		ambientTemperature_DegK = 0.0;			// Ambient temperature (kelvon)
	double		ambientDensity_KgPerM3	= 0.0;			// Ambient density (kg/m^3)
	double		wingSpan_FT				= 32.667;		// F-16 wing-span (ft)
	double		wingArea_FT2			= 300.0;		// F-16 wing area (ft^2)
	double		meanChord_FT			= 11.32;		// F-16 mean aerodynamic chord (ft)
	double		referenceCG_PCT			= 0.35;			// Reference CG as a % of wing chord
	double		actualCG_PCT			= 0.30;			// Actual CG as a % of wing chord
	double		pi						= acos(-1.0);	// Pi (3.14159....)
	double		radiansToDegrees		= 180.0/pi;		// Conversion factor from radians to degrees
	double		inertia_Ix_KGM2			= 12874.0;		// Reference moment of inertia (kg/m^2)
	double		inertia_Iy_KGM2			= 75673.6;		// Reference moment of inertia (kg/m^2)
	double		inertia_Iz_KGM2			= 85552.1;		// Reference moment of inertia (kg/m^2)
	double		temp[9];								// Temporary array for holding look-up table results
	double		altitude_FT				= 0.0;			// Absolute altitude MSL (ft)
	double		totalVelocity_FPS		= 0.0;			// Total velocity (always positive) (ft/s)
	double		alpha_DEG				= 0.0;			// Angle of attack (deg)
	double		beta_DEG				= 0.0;			// Slideslip angle (deg)
	double		rollRate_RPS			= 0.0;			// Body roll rate (rad/sec)
	double		pitchRate_RPS			= 0.0;			// Body pitch rate (rad/sec)
	double		yawRate_RPS				= 0.0;			// Body yaw rate (rad/sec)
	double		thrust_N				= 0.0;			// Engine thrust (N)
	double		elevator_DEG			= 0.0;			// Elevator deflection (deg)
	double		aileron_DEG				= 0.0;			// Aileron deflection (deg)
	double		rudder_DEG				= 0.0;			// Rudder  deflection (deg)
	double		elevator_DEG_commanded	= 0.0;			// Commanded elevator deflection from control system (deg)
	double		aileron_DEG_commanded	= 0.0;			// Commanded aileron deflection from control system (deg)
	double		rudder_DEG_commanded	= 0.0;			// Commanded rudder deflection from control system (deg)
	double		aileron_PCT				= 0.0;			// Aileron deflection as a percent of maximum (-1 to 1)
	double		rudder_PCT				= 0.0;			// Rudder deflection as a percent of maximum (-1 to 1)
	double		elevator_PCT			= 0.0;			// Elevator deflection as a percent of maximum (-1 to 1)
	double		leadingEdgeFlap_DEG		= 0.0;			// Leading edge flap deflection (deg)
	double		leadingEdgeFlap_PCT		= 0.0;			// Leading edge flap as a percent of maximum (0 to 1)
	double		dynamicPressure_LBFT2	= 0.0;			// Dynamic pressure (lb/ft^2)
	double		mach					= 0.0;			// Well..Mach, yeah
	double		ps_LBFT2				= 0.0;			// Ambient calculated pressure (lb/ft^2)
	double		meterToFoot				= 3.28084;		// Meter to foot conversion factor
	bool		simInitialized			= false;		// Has the simulation gone through it's first run frame?
	double		gearDown				= 0.0;			// Is the gear currently down?
	double		az						= 0.0;			// Az (per normal direction convention) out the bottom of the a/c (m/s^2)
	double		ay						= 0.0;			// Ay (per normal direction convention) out the right wing (m/s^2)
	double		pedInput				= 0.0;			// Pedal input command normalized (-1 to 1)
	double		throttleInput			= 0.0;			// Throttle input command normalized (-1 to 1)
	double		flap_DEG				= 0.0;			// Trailing edge flap deflection (deg)
	double		flap_PCT				= 0.0;			// Trailing edge flap deflection (0 to 1)
	double		weight_N				= 0.0;			// Weight force of aircraft (N)
	double		ay_world				= 0.0;			// World referenced up/down acceleration (m/s^2)
	double		weight_on_wheels		= 0.0;			// Weight on wheels flag (not used right now)
	double		rolling_friction		= 0.03;			// Rolling friction amount (not use right now)
	double		WheelBrakeCommand		= 0.0;			// Commanded wheel brake
	double		GearCommand				= 0.0;			// Commanded gear lever
	double		DeltaTime				= 0.0;			// Delta time of the simulation, in seconds
}

// These are taken from Export.lua
enum inputCommandsEnum
{
	LandingGearUp		= 430, // Gear up	(doesn't work)	
	LandingGearDown		= 431, // Gear down	(doesn't work)
	JoystickPitch		= 2001,	
	JoystickRoll		= 2002,
	JoystickYaw			= 2003,
	JoystickThrottle	= 2004,
	JoystickLeftEngineThrottle = 2005,
	JoystickRightEngineThrottle = 2006
}
F16inputs;

// Very important! This function sum up all the forces acting on
// the aircraft for this run frame.  It currently assume the force
// is acting at the CG
void add_local_force(const Vec3 & Force, const Vec3 & Force_pos)
{
	common_force.x += Force.x;
	common_force.y += Force.y;
	common_force.z += Force.z;
}

// Very important! This function sums up all the moments acting
// on the aircraft for this run frame.  It currently assumes the
// moment is acting at the CG
void add_local_moment(const Vec3 & Moment)
{
	common_moment.x += Moment.x;
	common_moment.y += Moment.y;
	common_moment.z += Moment.z;
}

// Not used but left in, it was part of the example
// functions to simulate weight change due to
// fuel consumption
void simulate_fuel_consumption(double dt)
{
	/*
		fuel_consumption_since_last_time =  10 * throttle * dt; //10 kg persecond
		if (fuel_consumption_since_last_time > internal_fuel)
			fuel_consumption_since_last_time = internal_fuel;
		internal_fuel -= fuel_consumption_since_last_time;
	 */
}

// This is where the simulation send the accumulated forces to the DCS Simulation
// after each run frame
void ed_fm_add_local_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{
	x = common_force.x;
	y = common_force.y;
	z = common_force.z;
	pos_x = center_of_gravity.x;
	pos_y = center_of_gravity.y;
	pos_z = center_of_gravity.z;
}

// Not used
void ed_fm_add_global_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{

}

// Not used
void ed_fm_add_global_moment(double & x,double &y,double &z)
{

}

// This is where the simulation send the accumulated moments to the DCS Simulation
// after each run frame
void ed_fm_add_local_moment(double & x,double &y,double &z)
{
	x = common_moment.x;
	y = common_moment.y;
	z = common_moment.z;
}

//-----------------------------------------------------------------------
// The most important part of the entire EFM code.  This is where you code
// gets called for each run frame.  Each run frame last for a duration of
// "dt" (delta time).  This can be used to help time certain features such
// as filters and lags
//-----------------------------------------------------------------------
void ed_fm_simulate(double dt)
{
	F16::DeltaTime = dt;

	/* Gear animation */
	if (F16::gearDown > F16::GearCommand)
	{
		F16::gearDown = limit(F16::gearDown - dt/2, 0.0, 1.0);
	} else if (F16::gearDown < F16::GearCommand)
	{
		F16::gearDown = limit(F16::gearDown + dt/2, 0.0 ,1.0);
	}; 

	/* CJS - Removed hack to filter out flight controller if on ground
	if(F16::weight_on_wheels)
	{
		F16::alpha_DEG = 0.0;
		F16::az = 0.0;
	}
	*/

	// Very important! clear out the forces and moments before you start calculated
	// a new set for this run frame
	common_force  = Vec3();
	common_moment = Vec3();

	// Get the total absolute velocity acting on the aircraft with wind included
	// using english units so airspeed is in feet/second here
	Vec3 airspeed;
	airspeed.x = velocity_world_cs.x - wind.x;
	airspeed.y = velocity_world_cs.y - wind.y;
	airspeed.z = velocity_world_cs.z - wind.z;

	F16::totalVelocity_FPS =  sqrt(airspeed.x * airspeed.x + airspeed.y * airspeed.y + airspeed.z * airspeed.z) * F16::meterToFoot;
	if(F16::totalVelocity_FPS < 0.01)
	{
		F16::totalVelocity_FPS = 0.01;
	}

	// Call the atmosphere model to get mach and dynamic pressure
	// I'm used to english units so I am using LB/FT^2 for the pressures
	double *temp;
	temp = (double *)malloc(9*sizeof(double));
	F16::ATMOS::atmos(F16::ambientTemperature_DegK,F16::ambientDensity_KgPerM3,F16::totalVelocity_FPS,temp);		
	F16::dynamicPressure_LBFT2	= temp[0];
	F16::mach = temp[1];

	//---------------------------------------------
	//-----CONTROL DYNAMICS------------------------
	//---------------------------------------------

	// Call the leading edge flap dynamics controller, this controller is based on dynamic pressure and angle of attack
	// and is completely automatic
	F16::leadingEdgeFlap_DEG = F16::FLIGHTCONTROLS::leading_edge_flap_controller(F16::alpha_DEG,F16::dynamicPressure_LBFT2,F16::ps_LBFT2,dt);	
	F16::leadingEdgeFlap_PCT = limit(F16::leadingEdgeFlap_DEG / 25.0, 0.0, 1.0);	

	// Call the longitudinal (pitch) controller.  Takes the following inputs:
	// -Normalize long stick input
	// -Trimmed G offset
	// -Angle of attack (deg)
	// -Pitch rate (rad/sec)
	// -Differential command (from roll controller, not quite implemented yet)
	F16::elevator_DEG_commanded   = -(F16::FLIGHTCONTROLS::fcs_pitch_controller(F16::FLIGHTCONTROLS::longStickInput,-0.3,F16::alpha_DEG,F16::pitchRate_RPS * F16::radiansToDegrees,(F16::az/9.81),0.0,F16::dynamicPressure_LBFT2,dt));
	// Call the servo dynamics model (not used as it causes high flutter in high speed situations, related to filtering and dt rate)
	F16::elevator_DEG	= F16::elevator_DEG_commanded; //F16::ACTUATORS::elevator_actuator(F16::elevator_DEG_commanded,dt);
	F16::elevator_DEG = limit(F16::elevator_DEG,-25.0,25.0);
	
	F16::aileron_DEG_commanded = (F16::FLIGHTCONTROLS::fcs_roll_controller(F16::FLIGHTCONTROLS::latStickInput,F16::FLIGHTCONTROLS::longStickForce,F16::ay/9.81,F16::rollRate_RPS* F16::radiansToDegrees,0.0,F16::dynamicPressure_LBFT2,dt));
	F16::aileron_DEG	= F16::aileron_DEG_commanded; //F16::ACTUATORS::aileron_actuator(F16::aileron_DEG_commanded,dt);
	F16::aileron_DEG = limit(F16::aileron_DEG,-21.5,21.5);

	F16::rudder_DEG_commanded = F16::FLIGHTCONTROLS::fcs_yaw_controller(	F16::pedInput, 0.0, F16::yawRate_RPS * (180.0/3.14159), F16::rollRate_RPS* F16::radiansToDegrees,
													F16::FLIGHTCONTROLS::alphaFiltered,F16::aileron_DEG_commanded,F16::ay/9.81,dt);
	F16::rudder_DEG		= F16::rudder_DEG_commanded; //F16::ACTUATORS::rudder_actuator(F16::rudder_DEG_commanded,dt);
	F16::rudder_DEG = limit(F16::rudder_DEG,-30.0,30.0);

	F16::flap_DEG = F16::FLIGHTCONTROLS::fcs_flap_controller(F16::totalVelocity_FPS);
	
	F16::thrust_N		= F16::ENGINE::engine_dynamics(F16::throttleInput,F16::mach,F16::altitude_FT,dt);	

	F16::aileron_PCT = F16::aileron_DEG / 21.5;
	F16::elevator_PCT = F16::elevator_DEG / 25.0;
	F16::rudder_PCT = F16::rudder_DEG / 30.0;
	F16::flap_PCT = F16::flap_DEG / 20.0;

	double alpha1_DEG_Limited	= limit(F16::alpha_DEG,-20.0,90.0);
	double beta1_DEG_Limited	= limit(F16::beta_DEG,-30.0,30.0);

	// FLAPS (From JBSim F16.xml config)
	double CLFlaps = 0.35 * F16::flap_PCT;
	double CDFlaps = 0.08 * F16::flap_PCT;

	double CzFlaps = - (CLFlaps * cos(F16::alpha_DEG * (F16::pi/180.0)) + CDFlaps * sin(F16::pi/180.0));
	double CxFlaps = - (-CLFlaps * sin(F16::alpha_DEG * (F16::pi/180.0)) + CDFlaps * cos(F16::pi/180.0));
	
	// TODO Speedbrakes aero (from JBSim F16.xml config)

	// TODO Gear aero (from JBSim F16.xml config)
	double CDGear = 0.0270 * F16::gearDown; 
	double CzGear = - (CDGear * sin(F16::pi/180.0));
	double CxGear = - (CDGear * cos(F16::pi/180.0));

	// TODO wheel friction
	/*
	double CxWheelFriction = 0.0;
	double CyWheelFriction = 0.0;
	if(F16::weight_on_wheels)
	{
		CxWheelFriction = -F16::rolling_friction * F16::weight_N;
		CyWheelFriction = 0.18 * F16::weight_N;
	}

	Vec3 cx_wheel_friction_force(CxWheelFriction, 0.0,0.0);
	Vec3 cx_wheel_friction_pos(0.0,0.0,0.0);
	add_local_force(cx_wheel_friction_force,cx_wheel_friction_pos);

	Vec3 cy_wheel_friction_force(0.0, 0.0,CyWheelFriction);
	Vec3 cy_wheel_friction_pos(0.0,0.0,0.0);
	add_local_force(cy_wheel_friction_force,cy_wheel_friction_pos);
	*/

	F16::AERO::hifi_C(alpha1_DEG_Limited,beta1_DEG_Limited,F16::elevator_DEG,temp);
		F16::AERO::Cx = temp[0];
		F16::AERO::Cz = temp[1];
		F16::AERO::Cm = temp[2];
		F16::AERO::Cy = temp[3];
		F16::AERO::Cn = temp[4];
		F16::AERO::Cl = temp[5];

	
	F16::AERO::hifi_damping(alpha1_DEG_Limited,temp);
        F16::AERO::Cxq = temp[0];
        F16::AERO::Cyr = temp[1];
        F16::AERO::Cyp = temp[2];
        F16::AERO::Czq = temp[3];
        F16::AERO::Clr = temp[4];
        F16::AERO::Clp = temp[5];
        F16::AERO::Cmq = temp[6];
        F16::AERO::Cnr = temp[7];
        F16::AERO::Cnp = temp[8];
	
    F16::AERO::hifi_C_lef(alpha1_DEG_Limited,beta1_DEG_Limited,temp);
        F16::AERO::Cx_delta_lef = temp[0];
        F16::AERO::Cz_delta_lef = temp[1];
        F16::AERO::Cm_delta_lef = temp[2];
        F16::AERO::Cy_delta_lef = temp[3];
        F16::AERO::Cn_delta_lef = temp[4];
        F16::AERO::Cl_delta_lef = temp[5];
	
    F16::AERO::hifi_damping_lef(alpha1_DEG_Limited,temp);
        F16::AERO::Cxq_delta_lef = temp[0];
        F16::AERO::Cyr_delta_lef = temp[1];
        F16::AERO::Cyp_delta_lef = temp[2];
        F16::AERO::Czq_delta_lef = temp[3];
        F16::AERO::Clr_delta_lef = temp[4];
        F16::AERO::Clp_delta_lef = temp[5];
        F16::AERO::Cmq_delta_lef = temp[6];
        F16::AERO::Cnr_delta_lef = temp[7];
        F16::AERO::Cnp_delta_lef = temp[8];
	
    F16::AERO::hifi_rudder(alpha1_DEG_Limited,beta1_DEG_Limited,temp);
        F16::AERO::Cy_delta_r30 = temp[0];
        F16::AERO::Cn_delta_r30 = temp[1];
        F16::AERO::Cl_delta_r30 = temp[2];

    F16::AERO::hifi_ailerons(alpha1_DEG_Limited,beta1_DEG_Limited,temp);
        F16::AERO::Cy_delta_a20     = temp[0];
        F16::AERO::Cy_delta_a20_lef = temp[1];
        F16::AERO::Cn_delta_a20     = temp[2];
        F16::AERO::Cn_delta_a20_lef = temp[3];
        F16::AERO::Cl_delta_a20     = temp[4];
        F16::AERO::Cl_delta_a20_lef = temp[5];

	F16::AERO::hifi_other_coeffs(alpha1_DEG_Limited,F16::elevator_DEG,temp);
        F16::AERO::Cn_delta_beta = temp[0];
        F16::AERO::Cl_delta_beta = temp[1];
        F16::AERO::Cm_delta      = temp[2];
        F16::AERO::eta_el        = temp[3];
        F16::AERO::Cm_delta_ds   = 0;        /* ignore deep-stall effect */

	/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	compute Cx_tot, Cz_tot, Cm_tot, Cy_tot, Cn_tot, and Cl_total
	(as on NASA report p37-40)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

	/* XXXXXXXX Cx_tot XXXXXXXX */

	F16::AERO::dXdQ = (F16::meanChord_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cxq + F16::AERO::Cxq_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cx_total = F16::AERO::Cx + F16::AERO::Cx_delta_lef*F16::leadingEdgeFlap_PCT + F16::AERO::dXdQ*F16::pitchRate_RPS;
	F16::AERO::Cx_total += CxFlaps + CxGear;
	/* ZZZZZZZZ Cz_tot ZZZZZZZZ */ 

	F16::AERO::dZdQ = (F16::meanChord_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Czq + F16::AERO::Cz_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cz_total = F16::AERO::Cz + F16::AERO::Cz_delta_lef*F16::leadingEdgeFlap_PCT + F16::AERO::dZdQ*F16::pitchRate_RPS;
	F16::AERO::Cz_total += CzFlaps + CzGear;
	/* MMMMMMMM Cm_tot MMMMMMMM */ 

	F16::AERO::dMdQ = (F16::meanChord_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cmq + F16::AERO::Cmq_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cm_total = F16::AERO::Cm*F16::AERO::eta_el + F16::AERO::Cz_total*(F16::referenceCG_PCT-F16::actualCG_PCT) + F16::AERO::Cm_delta_lef*F16::leadingEdgeFlap_PCT + F16::AERO::dMdQ*F16::pitchRate_RPS + F16::AERO::Cm_delta + F16::AERO::Cm_delta_ds;

	/* YYYYYYYY Cy_tot YYYYYYYY */


	F16::AERO::dYdail = F16::AERO::Cy_delta_a20 + F16::AERO::Cy_delta_a20_lef*F16::leadingEdgeFlap_PCT;

	F16::AERO::dYdR = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cyr + F16::AERO::Cyr_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::dYdP = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cyp + F16::AERO::Cyp_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cy_total = F16::AERO::Cy + F16::AERO::Cy_delta_lef*F16::leadingEdgeFlap_PCT + F16::AERO::dYdail*F16::aileron_PCT + F16::AERO::Cy_delta_r30*F16::rudder_PCT + F16::AERO::dYdR*F16::yawRate_RPS + F16::AERO::dYdP*F16::rollRate_RPS;
	
	/* NNNNNNNN Cn_tot NNNNNNNN */ 

	F16::AERO::dNdail = F16::AERO::Cn_delta_a20 + F16::AERO::Cn_delta_a20_lef*F16::leadingEdgeFlap_PCT;

	F16::AERO::dNdR = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cnr + F16::AERO::Cnr_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::dNdP = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Cnp + F16::AERO::Cnp_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cn_total = F16::AERO::Cn + F16::AERO::Cn_delta_lef*F16::leadingEdgeFlap_PCT - F16::AERO::Cy_total*(F16::referenceCG_PCT-F16::actualCG_PCT)*(F16::meanChord_FT/F16::wingSpan_FT) + F16::AERO::dNdail*F16::aileron_PCT + F16::AERO::Cn_delta_r30*F16::rudder_PCT + F16::AERO::dNdR*F16::yawRate_RPS + F16::AERO::dNdP*F16::rollRate_RPS + F16::AERO::Cn_delta_beta*F16::beta_DEG;

	/* LLLLLLLL Cl_total LLLLLLLL */

	F16::AERO::dLdail = F16::AERO::Cl_delta_a20 + F16::AERO::Cl_delta_a20_lef*F16::leadingEdgeFlap_PCT;

	F16::AERO::dLdR = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Clr + F16::AERO::Clr_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::dLdP = (F16::wingSpan_FT/(2*F16::totalVelocity_FPS))*(F16::AERO::Clp + F16::AERO::Clp_delta_lef*F16::leadingEdgeFlap_PCT);

	F16::AERO::Cl_total = F16::AERO::Cl + F16::AERO::Cl_delta_lef*F16::leadingEdgeFlap_PCT + F16::AERO::dLdail*F16::aileron_PCT + F16::AERO::Cl_delta_r30*F16::rudder_PCT + F16::AERO::dLdR*F16::yawRate_RPS + F16::AERO::dLdP*F16::rollRate_RPS + F16::AERO::Cl_delta_beta*F16::beta_DEG;

	//----------------------------------------------------------------
	// All prior forces calculated in lbs, needs to be converted
	// to units.  All prior forces calculated in lb*ft, needs
	// to be converted into N*m
	//----------------------------------------------------------------

	// Cy	(force out the right wing)
	Vec3 cy_force(0.0, 0.0 , F16::AERO::Cy_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * 4.44822162825  );		// Output force in Newtons
	Vec3 cy_force_pos(0.0,0,0); //0.01437
	add_local_force(cy_force,cy_force_pos);	

	// Cx (force out the nose)
	Vec3 cx_force(F16::AERO::Cx_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * 4.44822162825  , 0 , 0 );		// Output force in Newtons
	Vec3 cx_force_pos(0, 0.0,0.0);
	add_local_force(cx_force,cx_force_pos);

	// Cz (force down the bottom of the aircraft)
	Vec3 cz_force(0.0,  -F16::AERO::Cz_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * 4.44822162825 , 0.0 );	// Output force in Newtons
	Vec3 cz_force_pos(0,0,0);
	add_local_force(cz_force,cz_force_pos);

	// Cl	(Output force in N/m)
	Vec3 cl_moment( F16::AERO::Cl_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * F16::wingSpan_FT *1.35581795 , 0.0 ,  0.0  );
	add_local_moment(cl_moment);
	
	// Cm	(Output force in N/m)
	Vec3 cm_moment(0.0, 0.0 ,  F16::AERO::Cm_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * 1.35581795 * F16::meanChord_FT );
	add_local_moment(cm_moment);

	// Cn	(Output force in N/m)
	Vec3 cn_moment(0.0, -F16::AERO::Cn_total * F16::wingArea_FT2 * F16::dynamicPressure_LBFT2 * F16::wingSpan_FT *1.35581795 ,  0.0   );
	add_local_moment(cn_moment);	

	// Thrust	
	Vec3 thrust_force(F16::thrust_N , 0.0, 0.0);	// Output force in Newtons
	Vec3 thrust_force_pos(0,0,0);
	add_local_force(thrust_force,thrust_force_pos);	

	// Tell the simulation that it has gone through the first frame
	F16::simInitialized = true;
	F16::ACTUATORS::simInitialized = true;
	F16::FLIGHTCONTROLS::simInitialized = true;

	/*
	F16::weight_on_wheels = false;
	if((F16::weight_N > cz_force.y) && (abs(F16::ay_world) >= -0.5) && (F16::gearDown == 1.0))
	{
		F16::weight_on_wheels = true;
	}
	*/
	
}

void ed_fm_set_atmosphere(	double h,//altitude above sea level			(meters)
							double t,//current atmosphere temperature   (Kelvin)
							double a,//speed of sound					(meters/sec)
							double ro,// atmosphere density				(kg/m^3)
							double p,// atmosphere pressure				(N/m^2)
							double wind_vx,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
							double wind_vy,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
							double wind_vz //components of velocity vector, including turbulence in world coordinate system (meters/sec)
						)
{
	F16::ambientTemperature_DegK = t;
	F16::ambientDensity_KgPerM3 = ro;
	F16::altitude_FT = h * F16::meterToFoot;
	F16::ps_LBFT2 = p * 0.020885434273;
}

void ed_fm_set_current_mass_state ( double mass,
									double center_of_mass_x,
									double center_of_mass_y,
									double center_of_mass_z,
									double moment_of_inertia_x,
									double moment_of_inertia_y,
									double moment_of_inertia_z
									)
{
	center_of_gravity.x  = center_of_mass_x;
	center_of_gravity.y  = center_of_mass_y;
	center_of_gravity.z  = center_of_mass_z;

	inertia.x = moment_of_inertia_x;
	inertia.y = moment_of_inertia_y;
	inertia.z = moment_of_inertia_z;

	F16::weight_N = mass * 9.80665002864;
}
/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_state (double ax,//linear acceleration component in world coordinate system
							double ay,//linear acceleration component in world coordinate system
							double az,//linear acceleration component in world coordinate system
							double vx,//linear velocity component in world coordinate system
							double vy,//linear velocity component in world coordinate system
							double vz,//linear velocity component in world coordinate system
							double px,//center of the body position in world coordinate system
							double py,//center of the body position in world coordinate system
							double pz,//center of the body position in world coordinate system
							double omegadotx,//angular accelearation components in world coordinate system
							double omegadoty,//angular accelearation components in world coordinate system
							double omegadotz,//angular accelearation components in world coordinate system
							double omegax,//angular velocity components in world coordinate system
							double omegay,//angular velocity components in world coordinate system
							double omegaz,//angular velocity components in world coordinate system
							double quaternion_x,//orientation quaternion components in world coordinate system
							double quaternion_y,//orientation quaternion components in world coordinate system
							double quaternion_z,//orientation quaternion components in world coordinate system
							double quaternion_w //orientation quaternion components in world coordinate system
							)
{
	F16::ay_world = ay;
}


void ed_fm_set_current_state_body_axis(	double ax,//linear acceleration component in body coordinate system (meters/sec^2)
										double ay,//linear acceleration component in body coordinate system (meters/sec^2)
										double az,//linear acceleration component in body coordinate system (meters/sec^2)
										double vx,//linear velocity component in body coordinate system (meters/sec)
										double vy,//linear velocity component in body coordinate system (meters/sec)
										double vz,//linear velocity component in body coordinate system (meters/sec)
										double wind_vx,//wind linear velocity component in body coordinate system (meters/sec)
										double wind_vy,//wind linear velocity component in body coordinate system (meters/sec)
										double wind_vz,//wind linear velocity component in body coordinate system (meters/sec)
										double omegadotx,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegadoty,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegadotz,//angular accelearation components in body coordinate system (rad/sec^2)
										double omegax,//angular velocity components in body coordinate system (rad/sec)
										double omegay,//angular velocity components in body coordinate system (rad/sec)
										double omegaz,//angular velocity components in body coordinate system (rad/sec)
										double yaw,  //radians (rad)
										double pitch,//radians (rad/sec)
										double roll, //radians (rad/sec)
										double common_angle_of_attack, //AoA  (rad)
										double common_angle_of_slide   //AoS  (rad)
	)
{	
	velocity_world_cs.x = vx;
	velocity_world_cs.y = vy;
	velocity_world_cs.z = vz;

	wind.x = wind_vx;
	wind.y = wind_vy;
	wind.z = wind_vz;

	//-------------------------------
	// Start of setting F-16 states
	//-------------------------------
	F16::alpha_DEG		= common_angle_of_attack * F16::radiansToDegrees;
	F16::beta_DEG		= common_angle_of_slide * F16::radiansToDegrees;
	F16::rollRate_RPS	= omegax;
	F16::pitchRate_RPS	= omegaz;
	F16::yawRate_RPS	= -omegay;

	F16::az = ay;
	F16::ay = az;
}


void ed_fm_set_command (int command, float value)	// Command = Command Index (See Export.lua), Value = Signal Value (-1 to 1 for Joystick Axis)
{
	//----------------------------------
	// Set F-16 Raw Inputs
	//----------------------------------
	if(command == JoystickRoll)
	{
		F16::FLIGHTCONTROLS::latStickInput = limit(value,-1.0,1.0);
	}
	
	if(command == JoystickPitch)
	{
		F16::FLIGHTCONTROLS::longStickInput = limit(-value,-1.0,1.0);
	}

	if(command == JoystickYaw)
	{
		F16::pedInput = limit(-value,-1.0,1.0);
	}
	
	if(command == JoystickThrottle)
	{
		F16::throttleInput = limit(((-value + 1.0)/2.0) * 100.0,0.0,100.0);
	}
}
/*
	Mass handling 

	will be called  after ed_fm_simulate :
	you should collect mass changes in ed_fm_simulate 

	double delta_mass = 0;
	double x = 0;
	double y = 0; 
	double z = 0;
	double piece_of_mass_MOI_x = 0;
	double piece_of_mass_MOI_y = 0; 
	double piece_of_mass_MOI_z = 0;
 
	//
	while (ed_fm_change_mass(delta_mass,x,y,z,piece_of_mass_MOI_x,piece_of_mass_MOI_y,piece_of_mass_MOI_z))
	{
	//internal DCS calculations for changing mass, center of gravity,  and moments of inertia
	}
*/
bool ed_fm_change_mass  (double & delta_mass,
						double & delta_mass_pos_x,
						double & delta_mass_pos_y,
						double & delta_mass_pos_z,
						double & delta_mass_moment_of_inertia_x,
						double & delta_mass_moment_of_inertia_y,
						double & delta_mass_moment_of_inertia_z
						)
{
	if((inertia.x != F16::inertia_Ix_KGM2) ||
	   (inertia.y != F16::inertia_Iz_KGM2) ||
	   (inertia.z != F16::inertia_Iy_KGM2))
	{
		delta_mass = 0.0;
		delta_mass_pos_x = 0.0;
		delta_mass_pos_y = 0.0;
		delta_mass_pos_z = 0.0;
		delta_mass_moment_of_inertia_x = F16::inertia_Ix_KGM2 - inertia.x;
		delta_mass_moment_of_inertia_y = F16::inertia_Ix_KGM2 - inertia.z;
		delta_mass_moment_of_inertia_z = F16::inertia_Ix_KGM2 - inertia.y;

		// Can't set to true...crashing right now :(
		return false;
	}
	else
	{
		return false;
	}
	/*
	if (fuel_consumption_since_last_time > 0)
	{
		delta_mass		 = fuel_consumption_since_last_time;
		delta_mass_pos_x = -1.0;
		delta_mass_pos_y =  1.0;
		delta_mass_pos_z =  0;

		delta_mass_moment_of_inertia_x	= 0;
		delta_mass_moment_of_inertia_y	= 0;
		delta_mass_moment_of_inertia_z	= 0;

		fuel_consumption_since_last_time = 0; // set it 0 to avoid infinite loop, because it called in cycle 
		// better to use stack like structure for mass changing 
		return true;
	}
	else 
	{
		return false;
	}
	*/
}
/*
	set internal fuel volume , init function, called on object creation and for refueling , 
	you should distribute it inside at different fuel tanks
*/
void   ed_fm_set_internal_fuel(double fuel)
{
	internal_fuel = fuel;
}
/*
	get internal fuel volume 
*/
double ed_fm_get_internal_fuel()
{
	return internal_fuel;
}
/*
	set external fuel volume for each payload station , called for weapon init and on reload
*/
void  ed_fm_set_external_fuel (int	 station,
								double fuel,
								double x,
								double y,
								double z)
{

}
/*
	get external fuel volume 
*/
double ed_fm_get_external_fuel ()
{
	return 0;
}

void ed_fm_set_draw_args (EdDrawArgument * drawargs,size_t size)
{
	/* F-16 New Model Values Because I Am Back at Keeping Standards 
	double aileron_DrawPCT = 0.0;

	//Flaps
	drawargs[9].f  = (float)F16::flap_PCT;
	drawargs[10].f = (float)F16::flap_PCT;

	if(F16::aileron_PCT < 0.0)
	{
		aileron_DrawPCT = 0.5 * F16::aileron_PCT;
	}
	else
	{
		aileron_DrawPCT = F16::aileron_PCT;
	}

	drawargs[11].f = (float)aileron_DrawPCT + F16::flap_PCT;
	drawargs[12].f = (float)-aileron_DrawPCT + (F16::flap_PCT/2.0);

	drawargs[13].f   = (float)F16::leadingEdgeFlap_PCT;
	drawargs[14].f   = (float)F16::leadingEdgeFlap_PCT;

	drawargs[15].f = (float)-F16::elevator_PCT + limit(0.25 * aileron_DrawPCT,-5.0,5.0);
	drawargs[16].f = (float)-F16::elevator_PCT - limit(0.25 * aileron_DrawPCT,-5.0,5.0);

	drawargs[17].f = (float)-F16::rudder_PCT;
	drawargs[18].f = (float)F16::rudder_PCT;

	drawargs[28].f   = (float)limit(((F16::throttleInput-80.0)/20.0),0.0,1.0);
	drawargs[29].f   = (float)limit(((F16::throttleInput-80.0)/20.0),0.0,1.0);
	*/
	if (F16::simInitialized)
	{
		F16::GearCommand = drawargs[500].f;
	} else {
		drawargs[500].f = (float)F16::GearCommand;
	}
	F16::WheelBrakeCommand = drawargs[501].f;	
	F16::weight_on_wheels = limit(drawargs[504].f + drawargs[505].f + drawargs[506].f, 0.0, 1.0);
	// Just going to use wheight on any wheel...

	//Flaps
	drawargs[9].f = (float)F16::flap_PCT;
	drawargs[10].f = (float)F16::flap_PCT;

	drawargs[11].f = (float)-F16::aileron_PCT;
	drawargs[12].f = (float) F16::aileron_PCT;

	drawargs[13].f   = (float)F16::leadingEdgeFlap_PCT;
	drawargs[14].f   = (float)F16::leadingEdgeFlap_PCT;

	drawargs[15].f = (float)-F16::elevator_PCT;
	drawargs[16].f = (float)-F16::elevator_PCT;

	drawargs[17].f = (float) F16::rudder_PCT;
	drawargs[18].f = (float)-F16::rudder_PCT;


	drawargs[28].f   = (float)limit(((F16::throttleInput-80.0)/20.0),0.0,1.0);
	drawargs[29].f   = (float)limit(((F16::throttleInput-80.0)/20.0),0.0,1.0);
}

void ed_fm_configure(const char * cfg_path)
{

}

double ed_fm_get_param(unsigned index)
{	
	/* Gear positions */
	if (index == ED_FM_SUSPENSION_0_GEAR_POST_STATE)
	{
		return F16::gearDown;
	}
	if (index == ED_FM_SUSPENSION_1_GEAR_POST_STATE)
	{
		return F16::gearDown;
	}
	if (index == ED_FM_SUSPENSION_2_GEAR_POST_STATE)
	{
		return F16::gearDown;
	}
	if (index == ED_FM_SUSPENSION_0_DOWN_LOCK)
	{
		return F16::gearDown;
	}
	if (index == ED_FM_SUSPENSION_1_DOWN_LOCK)
	{
		return F16::gearDown;
	}
	if (index == ED_FM_SUSPENSION_2_DOWN_LOCK)
	{
		return F16::gearDown;
	}

	/* Gear brakes */
	if (index == ED_FM_SUSPENSION_1_RELATIVE_BRAKE_MOMENT)
	{
		return F16::WheelBrakeCommand * 1;
	}
	if (index == ED_FM_SUSPENSION_2_RELATIVE_BRAKE_MOMENT)
	{
		return F16::WheelBrakeCommand * 1;
	}


	/* Nosewheel steering */
	if (index == ED_FM_SUSPENSION_0_WHEEL_YAW)
	{
		return limit(F16::rudder_DEG * 0.012,-1.0,1.0);
	}

	if (index <= ED_FM_END_ENGINE_BLOCK)
	{
		switch (index)
		{
		case ED_FM_ENGINE_0_RPM:			
		case ED_FM_ENGINE_0_RELATED_RPM:	
		case ED_FM_ENGINE_0_THRUST:			
		case ED_FM_ENGINE_0_RELATED_THRUST:	
			return 0; // APU
		case ED_FM_ENGINE_1_RPM:
			return (F16::throttleInput/100.0) * 3000;
		case ED_FM_ENGINE_1_RELATED_RPM:
			return (F16::throttleInput/100.0);
		case ED_FM_ENGINE_1_THRUST:
			return (F16::throttleInput/100.0) * 5000 * 9.81;
		case ED_FM_ENGINE_1_RELATED_THRUST:
			return (F16::throttleInput/100.0);
		}
	}
	return 0;	
}


void ed_fm_cold_start()
{
	//F16::gearDown = 1;
	//F16::GearCommand = 1;
}

void ed_fm_hot_start()
{
	//F16::gearDown = 1;
	//F16::GearCommand = 1;
}

void ed_fm_hot_start_in_air()
{

}

double test()
{
	return 10.0;
}



