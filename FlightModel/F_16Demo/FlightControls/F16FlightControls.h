#ifndef __F16FLIGHTCONTROLS__
#define __F16FLIGHTCONTROLS__

#include "../stdafx.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include "../UtilityFunctions.h"
#include "../include/general_filter.h"

namespace F16
{
	namespace FLIGHTCONTROLS
	{
		bool		simInitialized = false;

		double		leading_edge_flap_integral					= 0.0;
		double		leading_edge_flap_integrated				= 0.0;
		double		leading_edge_flap_rate						= 0.0;
		double		leading_edge_flap_integrated_gained			= 0.0;
		double		leading_edge_flap_integrated_gained_biased	= 0.0;

		// Control filters (general filters to easily code up when compared to report block diagrams)
		GeneralFilter	pitchRateWashout;
		GeneralFilter	pitchIntegrator;
		GeneralFilter	pitchPreActuatorFilter;
		GeneralFilter	pitchActuatorDynamicsFilter;
		GeneralFilter	accelFilter;
		GeneralFilter	latStickForceFilter;
		GeneralFilter	rollCommandFilter;
		GeneralFilter	rollActuatorDynamicsFilter;
		GeneralFilter	rollRateFilter1;
		GeneralFilter	rollRateFilter2;
		GeneralFilter	rudderCommandFilter;
		GeneralFilter	yawRateWashout;
		GeneralFilter	yawRateFilter;
		GeneralFilter	yawServoFilter;

		// Pitch controller variables
		double		longStickInput			= 0.0;
		
		double		stickCommandPosFiltered = 0.0;
		double		azFiltered				= 0.0;
		double		alphaFiltered			= 0.0;
		double		longStickForce			= 0.0;
		double		latStickInput			= 0.0;

		// Controller for the leading edge flaps
		double leading_edge_flap_controller(double alpha_DEG, double dynamicPressure_FTLB, double staticPressure_FTLB, double frameTime)
		{
			double qbarOverPs = dynamicPressure_FTLB/staticPressure_FTLB;

			if(!(simInitialized))
			{
				leading_edge_flap_integral = -alpha_DEG;
				leading_edge_flap_integrated = leading_edge_flap_integral + 2*alpha_DEG;
				return leading_edge_flap_integral;
			}

			leading_edge_flap_rate = (alpha_DEG - leading_edge_flap_integrated) * 7.25;
			leading_edge_flap_integral += (leading_edge_flap_rate * frameTime);

			leading_edge_flap_integrated = leading_edge_flap_integral + alpha_DEG * 2.0;
			leading_edge_flap_integrated_gained = leading_edge_flap_integrated * 1.38;
			leading_edge_flap_integrated_gained_biased = leading_edge_flap_integrated_gained + 1.45 - (9.05 * qbarOverPs);	

			return leading_edge_flap_integrated_gained_biased; 
		}

		// Controller for yaw
		double fcs_yaw_controller(double pedInput, double pedTrim, double yaw_rate, double roll_rate, double aoa_filtered, double aileron_commanded, double ay, double dt)
		{
			if(!(simInitialized))
			{
				double numerators[2] = {0.0,4.0};
				double denominators[2] = {1.0,4.0};
				rudderCommandFilter.InitFilter(numerators,denominators,1,dt);

				double numerators1[2] = {1.0,0.0};
				double denominators1[2] = {1.0,1.0};
				yawRateWashout.InitFilter(numerators1,denominators1,1,dt);

				double numerators2[2] = {3.0,15.0};
				double denominators2[2] = {1.0,15.0};
				yawRateFilter.InitFilter(numerators2,denominators2,1,dt);

				double numerators3[3] = { 0.0, 0.0, pow(52.0,2.0)};
				double denomiantors3[3] = { 1.0, 2.0*0.7*52.0, pow(52.0,2.0)};
				yawServoFilter.InitFilter(numerators3,denomiantors3,2,dt);
			}

			double rudderForceCommand = pedInput * 450.0;

			double rudderCommand = 0.0;
			if(abs(rudderForceCommand) < 44.0)
			{
				rudderCommand = 0.0;
			}
			else if(rudderForceCommand >= 44.0)
			{
				rudderCommand = -0.0739 * rudderForceCommand + 3.2512;
			}
			else if(rudderForceCommand <= -44.0)
			{
				rudderCommand = -0.0739 * rudderForceCommand - 3.2512;
			}

			rudderCommand = limit(rudderCommand, -30.0, 30.0);
			double rudderCommandFiltered = rudderCommandFilter.Filter(!(simInitialized),dt,rudderCommand);
			double rudderCommandFilteredWTrim = pedTrim - rudderCommandFiltered;

			double alphaGained = aoa_filtered * (1.0/57.3);
			double rollRateWithAlpha = roll_rate * alphaGained;
			double yawRateWithRoll = yaw_rate - rollRateWithAlpha;

			double yawRateWithRollWashedOut = yawRateWashout.Filter(!(simInitialized),dt,yawRateWithRoll);
			double yawRateWithRollFiltered = yawRateFilter.Filter(!(simInitialized),dt,yawRateWithRollWashedOut);

			double yawRateFilteredWithSideAccel = yawRateWithRollFiltered;// + (ay * 19.3);

			double aileronGained = limit(0.05 * aoa_filtered, 0.0, 1.5) * aileron_commanded;

			double finalRudderCommand = aileronGained + yawRateFilteredWithSideAccel + rudderCommandFilteredWTrim;

			return finalRudderCommand;

			//TODO: Figure out why there is a ton of flutter at high speed due to these servo dynamics
			//double yawServoCommand = yawServoFilter.Filter(!(simInitialized),dt,finalRudderCommand);
			//return yawServoCommand;
		}

		// Stick force schedule for pitch control
		double fcs_pitch_controller_force_command(double longStickInput, double pitchTrim, double dt)
		{
			double longStickInputForce = 0.0;
			if(longStickInput > 0.0)
			{
				longStickInputForce = longStickInput * 80.0;
			}
			else
			{
				longStickInputForce = longStickInput * 180.0;
			}
			longStickInputForce = limit(longStickInputForce,-180.0,80.0);
			longStickForce = longStickInputForce;

			double longStickCommand_G = 0.0;
			if(abs(longStickInputForce) <=  8.0)
			{
				longStickCommand_G = 0.0;
			}
			else if((longStickInputForce < -8) && (longStickInputForce > -33.0))
			{
				longStickCommand_G = (0.016 * longStickInputForce) + 0.128;
			}
			else if(longStickInputForce <= -33.0)
			{
				longStickCommand_G = (0.067 * longStickInputForce) + 1.8112;
			}
			else if((longStickInputForce > 8.0) && (longStickInputForce < 33.0))
			{
				longStickCommand_G = (0.032 * longStickInputForce) - 0.256;
			}
			else if(longStickInputForce >= 33.0)
			{
				longStickCommand_G = 0.0681*longStickInputForce - 1.4468;
			}

			double longStickCommandWithTrim_G = pitchTrim - longStickCommand_G;

			double longStickCommandWithTrimLimited_G = limit(longStickCommandWithTrim_G, -4.0, 8.0);

			double longStickCommandWithTrimLimited_G_Rate = 4.0 * (longStickCommandWithTrimLimited_G - stickCommandPosFiltered);
			stickCommandPosFiltered += (longStickCommandWithTrimLimited_G_Rate * dt);

			return stickCommandPosFiltered;
		}

		// Schedule gain component due to dynamic pressure
		double dynamic_pressure_schedule(double dynPressure_LBFT2)
		{
			double dynamicPressure_kNM2 = dynPressure_LBFT2 * 1.4881639/1000.0; //for kN/m^2
			double scheduleOutput = 0.0;

			if(dynamicPressure_kNM2 < 9.576)
			{
				scheduleOutput = 1.0;
			}
			else if((dynamicPressure_kNM2 >= 9.576) && (dynamicPressure_kNM2 <= 43.0))
			{
				scheduleOutput =  (-0.018 * dynamicPressure_kNM2) + 1.1719;
				//scheduleOutput =  (-0.0239 * dynamicPressure_kNM2) + 1.2292;
			}
			else if(dynamicPressure_kNM2 > 43.0)
			{
				scheduleOutput = -0.003 * dynamicPressure_kNM2 + 0.5277;
				//scheduleOutput = -0.001 * dynamicPressure_kNM2 + 0.2422;
			}

			scheduleOutput = limit(scheduleOutput,0.05,1.0);

			return scheduleOutput;
		}

		// Angle of attack limiter logic
		double angle_of_attack_limiter(double alphaFiltered, double pitchRateCommand)
		{
			double topLimit = limit((alphaFiltered - 22.5) * 0.69,0.0,99999.0);
			double bottomLimit = limit((alphaFiltered - 15.0 + pitchRateCommand) * 0.322,0.0,99999.0);

			return (topLimit + bottomLimit);
		}

		// Controller for pitch
		double fcs_pitch_controller(double longStickInput, double pitchTrim, double angle_of_attack_ind, double pitch_rate, double az, double differentialCommand, double dynPressure_LBFT2, double dt)
		{
			if(!(simInitialized))
			{
				double numerators[2] = {1.0,0.0};
				double denominators[2] = {1.0,1.0};
				pitchRateWashout.InitFilter(numerators,denominators,1,dt);

				numerators[0] = 0.0; numerators[1] = 2.5;
				denominators[0] = 1.0; denominators[1] = 0.0;
				pitchIntegrator.InitFilter(numerators,denominators,1,dt);

				numerators[0] = 3.0; numerators[1] = 15;
				denominators[0] = 1.0; denominators[1] = 15.0;
				pitchPreActuatorFilter.InitFilter(numerators,denominators,1,dt);

				double numerators2[3] = { 0.0, 0.0, pow(52.0,2.0)};
				double denomiantors2[3] = { 1.0, 2.0*0.7*52.0, pow(52.0,2.0)};
				pitchActuatorDynamicsFilter.InitFilter(numerators2,denomiantors2,2,dt);

				numerators[0] = 0.0; numerators[1] = 15.0;
				denominators[0] = 1.0; denominators[1] = 15.0;
				accelFilter.InitFilter(numerators,denominators,1,dt);
			}

			double stickCommandPos =  fcs_pitch_controller_force_command(longStickInput, pitchTrim, dt);

			double dynamicPressureScheduled = dynamic_pressure_schedule(dynPressure_LBFT2);	

			azFiltered = accelFilter.Filter(!(simInitialized),dt,az-1.0);

			double alphaLimited = limit(angle_of_attack_ind,-5.0, 30.0);
			double alphaLimitedRate = 10.0 * (alphaLimited - alphaFiltered);
			alphaFiltered += (alphaLimitedRate * dt);

			double pitchRateWashedOut = pitchRateWashout.Filter(!(simInitialized),dt,pitch_rate);
			double pitchRateCommand = pitchRateWashedOut * 0.7 * dynamicPressureScheduled;		

			double limiterCommand = angle_of_attack_limiter(-alphaFiltered, pitchRateCommand);

			double gLimiterCommand = -(azFiltered +  (pitchRateWashedOut * 0.2));	

			double finalCombinedCommand = dynamicPressureScheduled * (2.5 * (stickCommandPos + limiterCommand + gLimiterCommand));

			double finalCombinedCommandFilteredLimited = limit(pitchIntegrator.Filter(!(simInitialized),dt,finalCombinedCommand),-25.0,25.0);
			finalCombinedCommandFilteredLimited = finalCombinedCommandFilteredLimited + finalCombinedCommand;

			double finalPitchCommandTotal = pitchPreActuatorFilter.Filter(!(simInitialized),dt,finalCombinedCommandFilteredLimited);
			finalPitchCommandTotal += (0.5 * alphaFiltered);

			return finalPitchCommandTotal;

			// TODO: There are problems with flutter with the servo dynamics...needs to be nailed down!
			//double actuatorDynamicsResult = pitchActuatorDynamicsFilter.Filter(!(simInitialized),dt,finalPitchCommandTotal);
			//return actuatorDynamicsResult;	
		}

		// Controller for roll
		double fcs_roll_controller(double latStickInput, double longStickForce, double ay, double roll_rate, double roll_rate_trim,double dynPressure_LBFT2, double dt)
		{
			if(!(simInitialized))
			{
				double numerators[2] = {0.0,60.0};
				double denominators[2] = {1.0,60.0};
				latStickForceFilter.InitFilter(numerators,denominators,1,dt);

				double numerators1[2] = {0.0,10.0};
				double denominators1[2] = {1.0,10.0};
				rollCommandFilter.InitFilter(numerators1,denominators1,1,dt);

				double numerators2[3] = { 0.0, 0.0, pow(52.0,2.0)};
				double denomiantors2[3] = { 1.0, 2.0*0.7*52.0, pow(52.0,2.0)};
				rollActuatorDynamicsFilter.InitFilter(numerators2,denomiantors2,2,dt);

				double numerators3[2] = {0.0,50.0};
				double denominators3[2] = {1.0,50.0};
				rollRateFilter1.InitFilter(numerators3,denominators3,1,dt);

				double numerators4[3] = { 4.0, 64.0, 6400.0};
				double denomiantors4[3] = { 1.0, 80.0, 6400.0};
				rollRateFilter2.InitFilter(numerators4,denomiantors4,2,dt);
			}

			double latStickForceCmd = latStickInput * 75.0;
			double latStickForce = latStickForceFilter.Filter(!(simInitialized),dt,latStickForceCmd);

			double latStickForceBiased = latStickForce - (ay * 8.9);  // CJS: remove side acceleration bias?

			double longStickForceGained = longStickForce * 0.0667;
			double rollFeelGain = 0.0;
			if(abs(longStickForce) > 25.0)
			{
				rollFeelGain = 0.7;
			}
			else if(longStickForce >= 0.0)
			{
				rollFeelGain = -0.012 * longStickForceGained + 1.0;
			}
			else if(longStickForce < 0.0)
			{
				rollFeelGain = 0.012 * longStickForceGained + 1.0;
			}

			double latStickForceFinal = latStickForceBiased * rollFeelGain;

			double rollRateCommand = 0.0;
			if(abs(latStickForceFinal) < 3.0)
			{
				rollRateCommand = 0.0;
			}
			else if((latStickForceFinal >= 3.0) && (latStickForceFinal <= 25.0))
			{
				rollRateCommand = 0.9091 * latStickForceFinal - 2.7273;
			}
			else if((latStickForceFinal > 25.0) && (latStickForceFinal <= 46.0))
			{
				rollRateCommand = 2.8571 * latStickForceFinal - 51.429;
			}
			else if((latStickForceFinal > 46.0))
			{
				rollRateCommand = 7.5862 * latStickForceFinal - 268.97;
			}
			else if((latStickForceFinal <= -3.0) && (latStickForceFinal >= -25.0))
			{
				rollRateCommand = 0.9091 * latStickForceFinal + 2.7273;
			}
			else if((latStickForceFinal < -25.0) && (latStickForceFinal >= -46.0))
			{
				rollRateCommand = 2.8571 * latStickForceFinal + 51.429;
			}
			else if((latStickForceFinal < -46.0))
			{
				rollRateCommand = 7.5862 * latStickForceFinal + 268.97;
			}

			double rollRateCommandFilterd = rollCommandFilter.Filter(!(simInitialized),dt,rollRateCommand);

			double rollRateFiltered1 = rollRateFilter1.Filter(!(simInitialized),dt,roll_rate);

			double rollRateFiltered2 = (rollRateFilter2.Filter(!(simInitialized),dt,rollRateFiltered1));

			double rollRateCommandCombined = rollRateFiltered2 - rollRateCommandFilterd - roll_rate_trim;

			double dynamicPressure_NM2 = dynPressure_LBFT2 * 47.880258889;

			double pressureGain = 0.0;
			if(dynamicPressure_NM2 < 19153.0)
			{
				pressureGain = 0.2;
			}
			else if((dynamicPressure_NM2 >= 19153.0) && (dynamicPressure_NM2 <= 23941.0))
			{
				pressureGain = -0.00002089 * dynamicPressure_NM2 + 0.6;
			}
			else
			{
				pressureGain = 0.1;
			}

			double rollCommandGained = limit(rollRateCommandCombined * pressureGain, -21.5, 21.5);

			// Mechanical servo dynamics
			double rollActuatorCommand = rollActuatorDynamicsFilter.Filter(!(simInitialized),dt,rollCommandGained);	
			return rollActuatorCommand;
		}		

		// Passive flap schedule for the F-16...nominal for now from flight manual comments
		double fcs_flap_controller(double airspeed_FPS)
		{
			double airspeed_KTS = 0.5924838012958964 * airspeed_FPS;
			double trailing_edge_flap_deflection = 0.0;

			if(airspeed_KTS < 240.0)
			{
				trailing_edge_flap_deflection = 20.0;
			}
			else if((airspeed_KTS >= 240.0) && (airspeed_KTS <= 370.0))
			{
				trailing_edge_flap_deflection = (1.0 - ((airspeed_KTS - 240.0)/(370.0-240.0))) * 20.0;
			}
			else
			{
				trailing_edge_flap_deflection = (1.0 - ((airspeed_KTS - 240.0)/(370.0-240.0))) * 20.0;
			}

			trailing_edge_flap_deflection = limit(trailing_edge_flap_deflection,0.0,20.0);

			return trailing_edge_flap_deflection;
		}
	}
}
#endif