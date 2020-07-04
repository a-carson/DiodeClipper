#pragma once
#ifndef DiodeClipper_h
#define DiodeClipper_h
#include <cmath>

template<class temp>

class DiodeClipper
{
public:

	DiodeClipper(){};

	void setSampleRate(temp sampleRate)
	{
		fs = sampleRate;
	}

	void setCircuitParameters(temp resistance, temp capacitance)
	{
		R = resistance;
		C = capacitance;
	}

	void setDiodeParameters(temp saturationCurrent, temp thermalVoltage, temp idealityFactor)
	{
		Is = saturationCurrent;
		Vt = thermalVoltage;
		Ni = idealityFactor;
	}

	void initialise()
	{
		cap = Ni * Vt * acosh((2.0f*fs*R*C + 1.0f) * Ni * Vt / (2.0f * Is * R));
		x = 0.0f;
		y = 0.0f;
		y_prev = 0.0f;
	}

	// Non linear function
	temp func(temp Y, temp p)
	{
		return Y*R*C + (Y + (2.0f*Is*R) * sinh(Y/(Ni*Vt)) - p) / (2.0f*fs);
	}

	// Derivative of Non-linear function
	temp dfunc(temp Y)
	{
		return R * C + (1 + (2 * Is * R / (Ni * Vt)) * cosh(Y / (Ni * Vt))) / (2.0f * fs);
	}

	// Main Process
	temp process(temp in)
	{
		iter = 0;
		const temp p = x + in;		
		y = Ni * Vt * asinh(p / (2 * Is * R));
		temp res = func(y, p);
		temp J = dfunc(y);
		temp step = res / J;
		temp cond = fabsf(step);

		while ((cond > tol) && (iter < maxIters))
		{
			// pop a cap
			if (step > cap)
			{
				step = cap;
			}

			if (step < -1.0f * cap)
			{
				step = -1.0f * cap;
			}

			y -= step;
			res = func(y, p);
		    J = dfunc(y);
			step = res / J;
			iter++;
			cond = fabsf(step);
		}

		// fail safe
		if (y != y)
		{
			y = 0;
		}

		x =  4.0f * fs * y * R * C - x;
		return y;
	}
private:

	// Sample Rate
	temp fs;

	// state variable
	temp x;

	// input variable
	temp u;

	// output variable
	temp y;
	temp y_prev;

	// Circuit parameters
	temp C;								// capactance
	temp R;								// resistance
	temp Is;								// saturation current
	temp Vt;								// thermal voltage
	temp Ni;								// ideality factor

	// Newton raphson parameters
	temp cap;
	const temp tol = 1e-7;						// tolerance
	const unsigned int maxIters = 25;  // maximum number of iterations
	int iter = 0;
};
#endif // !DiodeClipper_h

