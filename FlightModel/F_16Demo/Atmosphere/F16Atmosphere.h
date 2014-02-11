#include "../stdafx.h"

namespace F16
{
	namespace ATMOS
	{
		// Simple atmospheric calculations
		void atmos(double temperature, double density, double vt, double *coeff )
		{
			double temp, rho, mach, qbar;
			
			temp = temperature * 1.8; // In Deg Rankine
			rho = density * 0.00194032033;
			mach = (vt)/sqrt(1.4*1716.3*temp);
			qbar = .5*rho*pow(vt,2);
			
			coeff[0] = qbar;
			coeff[1] = mach;
		}	
	}
}