#ifndef __F16ACTUATORS__
#define __F16ACTUATORS__

#include "../stdafx.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include "../UtilityFunctions.h"

namespace F16
{
	namespace ACTUATORS
	{
		double	elevatorPosition_DEG	= 0.0;
		double	elevatorRate_DEGPERSEC	= 0.0;
		double	aileronPosition_DEG		= 0.0;
		double	aileronRate_DEGPERSEC	= 0.0;
		double	rudderPosition_DEG		= 0.0;
		double	rudderRate_DEGPERSEC	= 0.0;

		bool	simInitialized			= false;

		double  elevator_actuator(double elevatorCommanded_DEG, double frameTime)
		{
			if(!simInitialized)
			{
				elevatorPosition_DEG = elevatorCommanded_DEG;
				return elevatorPosition_DEG;		
			}

			elevatorRate_DEGPERSEC = 20.2 * (elevatorCommanded_DEG - elevatorPosition_DEG);

			elevatorRate_DEGPERSEC = limit(elevatorRate_DEGPERSEC,-60.0,60.0);

			elevatorPosition_DEG += (elevatorRate_DEGPERSEC * frameTime);

			elevatorPosition_DEG = limit(elevatorPosition_DEG, -25.0, 25.0);

			return elevatorPosition_DEG;
		}

		double  aileron_actuator(double aileronCommanded_DEG, double frameTime)
		{
			if(!simInitialized)
			{
				aileronPosition_DEG	 = aileronCommanded_DEG;
				return aileronPosition_DEG	;
			}

			aileronRate_DEGPERSEC = 20.2 * (aileronCommanded_DEG - aileronPosition_DEG	);

			aileronRate_DEGPERSEC = limit(aileronRate_DEGPERSEC,-56.0,56.0);

			aileronPosition_DEG	 += (aileronRate_DEGPERSEC * frameTime);

			aileronPosition_DEG = limit(aileronPosition_DEG, -21.5, 21.5);

			return aileronPosition_DEG;
		}

		double  rudder_actuator(double rudderCommanded_DEG, double frameTime)
		{
			if(!simInitialized)
			{
				rudderPosition_DEG	 = rudderCommanded_DEG;
				return rudderPosition_DEG	;
			}

			rudderRate_DEGPERSEC = 20.2 * (rudderCommanded_DEG - rudderPosition_DEG	);

			rudderRate_DEGPERSEC = limit(rudderRate_DEGPERSEC,-120.0,120.0);

			rudderPosition_DEG	 += (rudderRate_DEGPERSEC * frameTime);

			rudderPosition_DEG = limit(rudderPosition_DEG,-30.0,30.0);

			return rudderPosition_DEG;
		}
	}
}
#endif