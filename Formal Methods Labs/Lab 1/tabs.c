//Array index mnemonics for negative big, medium, and small, zero, and positive small, medium and big.
#define NB 0
#define NM 1
#define NS 2
#define ZE 3
#define PS 4
#define PM 5
#define PB 6

/*
 *	Optimum wheel slip under braking: S_ref = 0.15. Multiplied by 1000.
 */
#define S_ref 150

/*
 *	The control signal to hydraulic modulator is calculated periodically once
 *	every 20 ms. Multiplied by 1000.
 */
#define delta_t 20

/*
 *	The radius of the wheels is 0.25 m. Multiplied by 1000.
 */
#define R 250

/*
 *	The table u used to compute the control signal. First index is for error
 *	(variable e in the function compute_control_signal) and second index for
 *	error prime (variable ep in the function compute_control_signal). Each
 *	entry has been multiplied by 1000 and rounded to nearest integer.
 */
//					  	  	  NB	NM		NS		ZE		PS		PM		PB
const int u[7][7] = /*NB*/	{{1000,	1000,	1000,	1000,	667,	333,	0},
					/*NM*/	 {1000,	1000,	1000,	667,	667,	0,		-333},
					/*NS*/	 {1000,	667,	667,	333,	0,		-333,	-667},
					/*ZE*/	 {1000,	667,	333,	0,		-333,	-667,	-1000},
					/*PS*/	 {667,	333,	0,		-333,	-667,	-667,	-1000},
					/*PM*/	 {333,	0,		-667,	-667,	-1000,	-1000,	-1000},
					/*PB*/	 {0,	-333,	-667,	-1000,	-1000,	-1000,	-1000}};

/*@
	predicate u_init =
	u[NB][NB] == 1000 && u[NB][NM] == 1000 && u[NB][NS] == 1000 && u[NB][ZE] == 1000  && u[NB][PS] == 667   && u[NB][PM] == 333   && u[NB][PB] == 0     &&
	u[NM][NB] == 1000 && u[NM][NM] == 1000 && u[NM][NS] == 1000 && u[NM][ZE] == 667   && u[NM][PS] == 667   && u[NM][PM] == 0     && u[NM][PB] == -333  &&
	u[NS][NB] == 1000 && u[NS][NM] == 667  && u[NS][NS] == 667  && u[NS][ZE] == 333   && u[NS][PS] == 0     && u[NS][PM] == -333  && u[NS][PB] == -667  &&
	u[ZE][NB] == 1000 && u[ZE][NM] == 667  && u[ZE][NS] == 333  && u[ZE][ZE] == 0     && u[ZE][PS] == -333  && u[ZE][PM] == -667  && u[ZE][PB] == -1000 &&
	u[PS][NB] == 667  && u[PS][NM] == 333  && u[PS][NS] == 0    && u[PS][ZE] == -333  && u[PS][PS] == -667  && u[PS][PM] == -667  && u[PS][PB] == -1000 &&
	u[PM][NB] == 333  && u[PM][NM] == 0    && u[PM][NS] == -667 && u[PM][ZE] == -667  && u[PM][PS] == -1000 && u[PM][PM] == -1000 && u[PM][PB] == -1000 &&
	u[PB][NB] == 0    && u[PB][NM] == -333 && u[PB][NS] == -667 && u[PB][ZE] == -1000 && u[PB][PS] == -1000 && u[PB][PM] == -1000 && u[PB][PB] == -1000;
*/

/*
 *	Dummy variable representing the wheel velocity sensor in radians/s.
 *	Includes the factor 1000.
 */
int wt_sensor;

/*
 *	Dummy variable representing the vehicle acceleration sensor in m/s².
 *	Includes the factor 1000.
 */
int at_sensor;

/*
 *	Dummy variable representing the sensor of whether the brake pedal is pushed
 *	or not. Nonzero value is true and zero is false.
 */
int bp_sensor;

/*
 *	Stores the wheel slip value computed from the last computation of the
 *	control signal. That is, the last computed wheel slip value.
 */
int S_previous;

/*
 *	Stores the velocity of the vehicle just before braking. Written each time
 *	the top-level function hydraulic_modulator_driver is invoked and the brake
 *	pedal is not pushed. Used to compute the current velocity of the vehicle
 *	during braking.
 */
int velocity_before_braking;

/*
 *	Stores the sum of the acceleration samples of the vehicle read during
 *	braking. Written by hydraulic_modulator_driver. If the brake pedal is not
 *	pushed, then acceleration_sum is set to the current acceleration of the
 *	vehicle. If the brake pedal is pushed, then acceleration_sum is added with
 *	the current acceleration of the vehicle. Hence, acceleration_sum = Σa_i.
 *	acceleration_sum is used to compute the current velocity of the vehicle.
 */
int acceleration_sum;

/*
 *	Dummy variable representing the hydraulic modulator. This variable holds
 *	the value currently being sent to the hydraulic modulator.
 */
int signal_to_hydraulic_modulator;

/******************************************************************************
 * The Membership functions µₘ*************************************************
 ******************************************************************************/

/*
 *µNB(x) ≔ 1			if x ≤ -1
 *µNB(x) ≔ -2x - 1		if -1 < x < -0.5
 *µNB(x) ≔ 0			if -0.5 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes x <= -1000;
		ensures \result == 1000;
	behavior prop_7_2:
		assumes -1000 < x <= -500;
		ensures \result == (-2*x - 1000);
*/
int mNB(int x) {
	if (x <= -1000)
		return 1000;
	else if (-1000 < x && x <-500)
		return -2*x - 1000;
	else
		return 0;
}

/*
 *µNM(x) ≔ 0			if x ≤ -1
 *µNM(x) ≔ 2x + 2		if -1 < x ≤ -0.5
 *µNM(x) ≔ -4x - 1		if -0.5 < x < -0.25
 *µNM(x) ≔ 0			if -0.25 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes x <= -1000;
		ensures \result == 0;
	behavior prop_7_2:
		assumes -1000 < x <= -500;
		ensures \result == (2*x + 2000);
*/
int mNM(int x) {
	if (x <= -1000)
		return 0;
	else if (-1000 < x && x <= -500)
		return 2*x + 2000;
	else if (-500 < x && x < -250)
		return -4*x - 1000;
	else
		return 0;
}

/*
 *µNS(x) ≔ 0			if x ≤ -0.5
 *µNS(x) ≔ 4x + 2		if -0.5 < x ≤ -0.25
 *µNS(x) ≔ -4x			if -0.25 < x < 0
 *µNS(x) ≔ 0			if 0 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7:
		assumes x <= -500;
		ensures \result == 0;
*/
int mNS(int x) {
	if (x <= -500)
		return 0;
	else if (-500 < x && x <= -250)
		return 4*x + 2000;
	else if (-250 < x && x < 0)
		return -4*x;
	else
		return 0;
}

/*
 *µZE(x) ≔ 0			if x ≤ -0.25
 *µZE(x) ≔ 4x + 1		if -0.25 < x ≤ 0
 *µZE(x) ≔ -4x + 1		if 0 < x < 0.25
 *µZE(x) ≔ 0			if 0.25 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes -500 < x <= -250;
		ensures \result == 0;

	behavior prop_7_2:
		assumes x <= -500;
		ensures \result == 0;
*/
int mZE(int x) {
	//@ assert (x <= -500 <= -250) ==> (x <= -250);
	if (x <= -250)
		return 0;
	else if (-250 < x && x <= 0)
		return 4*x + 1000;
	else if (0 < x && x < 250)
		return -4*x + 1000;
	else
		return 0;
}

/*
 *µPS(x) ≔ 0			if x ≤ 0
 *µPS(x) ≔ 4x			if 0 < x ≤ 0.25
 *µPS(x) ≔ -4x + 2		if 0.25 < x < 0.5
 *µPS(x) ≔ 0			if 0.5 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes -500 < x <= 0;
		ensures \result == 0;

	behavior prop_7_2:
		assumes x <= -500;
		ensures \result == 0;
*/
int mPS(int x) {
	//@ assert (x <= -500 <= 0) ==> (x <= 0);
	if (x <= 0)
		return 0;
	else if (0 < x && x <= 250)
		return 4*x;
	else if (250 < x && x < 500)
		return -4*x + 2000;
	else
		return 0;
}

/*
 *µPM(x) ≔ 0			if x ≤ 0.25
 *µPM(x) ≔ 4x - 1		if 0.25 < x ≤ 0.5
 *µPM(x) ≔ -2x + 2		if 0.5 < x < 1
 *µPM(x) ≔ 0			if 1 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes -500 < x <= 250;
		ensures \result == 0;

	behavior prop_7_2:
		assumes x <= -500;
		ensures \result == 0;
*/
int mPM(int x) {
	//@ assert (x <= -500 <= 250) ==> (x <= 250);
	if (x <= 250)
		return 0;
	else if (250 < x && x <= 500)
		return 4*x - 1000;
	else if (500 < x && x < 1000)
		return -2*x + 2000;
	else
		return 0;
}

/*
 *µPB(x) ≔ 0			if x ≤ 0.5
 *µPB(x) ≔ 2x - 1 		if 0.5 < x < 1
 *µPB(x) ≔ 1			if 1 ≤ x
 */
/*@
	assigns \nothing;

	behavior prop_7_1:
		assumes -500 < x <= 500;
		ensures \result == 0;

	behavior prop_7_2:
		assumes x <= -500;
		ensures \result == 0;
*/
int mPB(int x) {
	//@ assert (x <= -500 <= 500) ==> (x <= 500);
	if (x <= 500)
		return 0;
	else if (500 < x && x < 1000)
		return 2*x - 1000;
	else
		return 1000;
}

/*
 *	Computes the membership degree.
 *	index ∈ {NB, NM, NS, ZE, PS, PM, PB}.
 *	x is e or ep.
 */
/*@
	requires x <= -500;
	assigns \nothing;

	behavior NB_1:
		assumes index == NB;
		assumes x <= -1000;
		ensures \result == 1000;
	behavior NB_2:
		assumes index == NB;
		assumes -1000 < x <= -500;
		ensures \result == (-2*x - 1000);

	behavior NM_1:
		assumes index == NM;
		assumes x <= -1000;
		ensures \result == 0;
	behavior NM_2:
		assumes index == NM;
		assumes -1000 < x <= -500;
		ensures \result == (2*x + 2000);

	behavior others:
		assumes index != NB && index != NM;
		ensures \result == 0;
*/
int md(int index, int x) {
	if (index == NB)
		return mNB(x);
	else if (index == NM)
		return mNM(x);
	else if (index == NS)
		return mNS(x);
	else if (index == ZE)
		return mZE(x);
	else if (index == PS)
		return mPS(x);
	else if (index == PM)
		return mPM(x);
	else
		//@ assert index != NB && index != NM;
		return mPB(x);
}

/******************************************************************************
 * End of Membership functions ************************************************
 ******************************************************************************/



/******************************************************************************
 * Dummy functions used to perform input/output *******************************
 ******************************************************************************/

/*
 *	Returns the current angular wheel velocity.
 */
/*@
	assigns \nothing;
	ensures \result == wt_sensor;
*/
int read_wheel_angular_velocity(void) {
	return wt_sensor;
}

/*
 *	Returns the current acceleration of the vehicle.
 */
/*@
	assigns \nothing;
	ensures \result == at_sensor;
*/
int read_acceleration_of_vehicle(void) {
	return at_sensor;
}

/*
 *	Returns non-zero if the brake pedal is pushed, and zero if the brake pedal
 *	is not pushed.
 */
/*@
	assigns \nothing;
	ensures \result == bp_sensor;
*/
int read_brake_pedal(void) {
	return bp_sensor;
}

/*
 *	Writes uc to the hydraulic modulator.
 */
/*@
	assigns signal_to_hydraulic_modulator;
	ensures \at(signal_to_hydraulic_modulator, Post) == \at(uc, Pre);
*/
void write_control_signal_to_hydraulic_modulator(int uc) {
	signal_to_hydraulic_modulator = uc;
}

/******************************************************************************
 * End of dummy functions used to perform input/output ************************
 ******************************************************************************/


/*
 *	Output: Velocity of vehicle = Σa*dt + v0 = v0 + dt⋅Σa.
 *	First term is divided by 1000 to keep the quantities in terms of 1000,
 *	since both acceleration_sum and delta_t are already multiplied by 1000.
 */
/*@
	assigns \nothing;
	ensures \result == ((acceleration_sum*delta_t/1000) + velocity_before_braking);
*/
int compute_velocity_of_vehicle(void) {
	return acceleration_sum*delta_t/1000 + velocity_before_braking;
}

/*
 *	v: Vehicle velocity m/s.
 *	wt: Angular wheel velocity radians/s.
 *
 *	Output: New wheel slip S.
 */
/*@
	assigns \nothing;
	ensures \result == (((v - wt*R/1000) * 1000) / v);
*/
int compute_wheel_slip(int v, int wt) {
	return ((v - wt*R/1000)*1000)/v;
}

/*
 *	Computes the control signal to the hydraulic modulator.
 */
/*@
	requires u_init;
	requires
		\let wt = wt_sensor;
		\let v = ((acceleration_sum*delta_t/1000) + velocity_before_braking);
		\let S = ((v - wt*R/1000)*1000)/v;
		S <= -350 && ((S - S_previous)*1000)/delta_t <= -500;
	assigns S_previous;
	ensures \result == 1000;
*/
int compute_control_signal(void) {
	int wt = read_wheel_angular_velocity();
	//@ assert wt == wt_sensor;

	int v = compute_velocity_of_vehicle();
	//@ assert v == ((acceleration_sum*delta_t/1000) + velocity_before_braking);

	int S = compute_wheel_slip(v, wt);
	//@ assert S == ((v - wt*R/1000)*1000)/v;

	int e = S - S_ref;									//error.
	//@ assert e == S - S_ref;
	//@ assert S <= -350;
	//@ assert e <= -500;

	int ep = ((S - S_previous)*1000)/delta_t;			//error prime.
	//@ assert ep == ((S - S_previous)*1000)/delta_t;
	//@ assert ep <= -500;

	S_previous = S;										//Updates old value of wheel slip.
	//@ assert S_previous == S;

	//Computes the control signal for each of the error and error derivative combinations.
	//The code below this for loop is equivalent to this for loop, hopefully. Except for scaling.
	//	for (error_index = 0; error_index < 7; error_index = error_index + 1)
	//		for (error_prime_index = 0; error_prime_index < 7; error_prime_index = error_prime_index + 1) {
	//			int m = md(error_index, error)*md(error_prime_index, error_prime);
	//			numerator += m*u_table[error_index][error_prime_index];
	//			denominator += m;
	//		}
	//	}

	int numerator = 0, denominator = 0;
	//@ assert numerator == 0 && denominator == 0;

	int ep_sum = md(NB, ep) + md(NM, ep) + md(NS, ep) + md(ZE, ep) + md(PS, ep) + md(PM, ep) + md(PB, ep);
	//@ assert ep <= -500;
	//@ assert (ep <= -500) ==> (ep_sum == 1000);
	//@ assert ep_sum == 1000;

	//error is NB. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(NB, e)*(md(NB, ep)*u[NB][NB] + md(NM, ep)*u[NB][NM] + md(NS, ep)*u[NB][NS] + md(ZE, ep)*u[NB][ZE] + md(PS, ep)*u[NB][PS] + md(PM, ep)*u[NB][PM] + md(PB, ep)*u[NB][PB])/1000;
	//@ assert (-1000 < e <= -500) && (ep <= -500) ==> (numerator == -2000*e - 1000000);
	//@ assert (e <= -1000) && (ep <= -500) ==> (numerator == 1000000);

	//error is NM. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(NM, e)*(md(NB, ep)*u[NM][NB] + md(NM, ep)*u[NM][NM] + md(NS, ep)*u[NM][NS] + md(ZE, ep)*u[NM][ZE] + md(PS, ep)*u[NM][PS] + md(PM, ep)*u[NM][PM] + md(PB, ep)*u[NM][PB])/1000;
	//@ assert (-1000 < e <= -500) && (ep <= -500) ==> (numerator == 1000000);
	//@ assert (e <= -1000) && (ep <= -500) ==> (numerator == 1000000);
	//@ assert numerator == 1000000;

	//error is NS. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(NS, e)*(md(NB, ep)*u[NS][NB] + md(NM, ep)*u[NS][NM] + md(NS, ep)*u[NS][NS] + md(ZE, ep)*u[NS][ZE] + md(PS, ep)*u[NS][PS] + md(PM, ep)*u[NS][PM] + md(PB, ep)*u[NS][PB])/1000;
	//@ assert numerator == 1000000;

	//error is ZE. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(ZE, e)*(md(NB, ep)*u[ZE][NB] + md(NM, ep)*u[ZE][NM] + md(NS, ep)*u[ZE][NS] + md(ZE, ep)*u[ZE][ZE] + md(PS, ep)*u[ZE][PS] + md(PM, ep)*u[ZE][PM] + md(PB, ep)*u[ZE][PB])/1000;
	//@ assert numerator == 1000000;

	//error is PS. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(PS, e)*(md(NB, ep)*u[PS][NB] + md(NM, ep)*u[PS][NM] + md(NS, ep)*u[PS][NS] + md(ZE, ep)*u[PS][ZE] + md(PS, ep)*u[PS][PS] + md(PM, ep)*u[PS][PM] + md(PB, ep)*u[PS][PB])/1000;
	//@ assert numerator == 1000000;

	//error is PM. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(PM, e)*(md(NB, ep)*u[PM][NB] + md(NM, ep)*u[PM][NM] + md(NS, ep)*u[PM][NS] + md(ZE, ep)*u[PM][ZE] + md(PS, ep)*u[PM][PS] + md(PM, ep)*u[PM][PM] + md(PB, ep)*u[PM][PB])/1000;
	//@ assert numerator == 1000000;

	//error is PB. Go through all of {NB, NM, NS, ZE, PS, PM, PB} for error prime.
	numerator += md(PB, e)*(md(NB, ep)*u[PB][NB] + md(NM, ep)*u[PB][NM] + md(NS, ep)*u[PB][NS] + md(ZE, ep)*u[PB][ZE] + md(PS, ep)*u[PB][PS] + md(PM, ep)*u[PB][PM] + md(PB, ep)*u[PB][PB])/1000;
	//@ assert numerator == 1000000;

	//@ assert ep_sum == 1000;
	denominator += md(NB, e)*ep_sum/1000;
	//@ assert (-1000 < e <= -500) ==> (denominator == -2*e -1000);
	//@ assert (e <= -1000) ==> (denominator == 1000);

	denominator += md(NM, e)*ep_sum/1000;
	//@ assert (-1000 < e <= -500) ==> (denominator == 1000);
	//@ assert (e <= -1000) ==> (denominator == 1000);
	//@ assert (denominator == 1000);

	denominator += md(NS, e)*ep_sum/1000;
	//@ assert (denominator == 1000);

	denominator += md(ZE, e)*ep_sum/1000;
	//@ assert (denominator == 1000);

	denominator += md(PS, e)*ep_sum/1000;
	//@ assert (denominator == 1000);

	denominator += md(PM, e)*ep_sum/1000;
	//@ assert (denominator == 1000);

	denominator += md(PB, e)*ep_sum/1000;
	//@ assert (denominator == 1000);

	//Returns uc.
	return numerator/denominator;
}

/*
 *	It is assumed that the interrupt service routine calls this main function
 *	each time a timer interrupt occurs (once every 0.02 seconds).
 */
/*@
	requires u_init;
	behavior for_pedal_not_pushed:
		assumes bp_sensor == 0;
		assigns velocity_before_braking;
		assigns acceleration_sum;
		assigns S_previous;
		assigns signal_to_hydraulic_modulator;
		ensures velocity_before_braking == (wt_sensor * R / 1000);
		ensures acceleration_sum == at_sensor;
		ensures S_previous == 0;
		ensures signal_to_hydraulic_modulator == -1000;
*/
void hydraulic_modulator_driver(void) {
	//The brake pedal is not pushed.
	if (read_brake_pedal() == 0) {
		//@ assert bp_sensor == 0;
		//Reads the current angular velocity of the wheel to compute the
		//current velocity of vehicle.
		int wt = read_wheel_angular_velocity();
		//@ assert wt == wt_sensor;
		velocity_before_braking = wt*R/1000;
		//@ assert velocity_before_braking == (wt * R / 1000);
		//Stores the current acceleration of the vehicle. The first time
		//hydraulic_modulator_driver is invoked when the brake pedal is pushed,
		//acceleration_sum is equal to the acceleration of the vehicle just
		//before braking. This means that the integration of the acceleration
		//over time is done over the time interval that starts when braking
		//starts.
		acceleration_sum = read_acceleration_of_vehicle();
		//@ assert acceleration_sum == at_sensor;
		//No wheel slip since the brakes are not applied and therefore the
		//wheels are rolling freely.
		S_previous = 0;
		//@ assert S_previous == 0;
		//Instructs the hydraulic modulator to not cause any brake pressure.
		write_control_signal_to_hydraulic_modulator(-1000);
		//@ assert signal_to_hydraulic_modulator == -1000;
	} else {
		//The brake pedal is pushed.
		//Adds the current acceleration of the vehicle.
		acceleration_sum += read_acceleration_of_vehicle();
		//Computes the control signal.
		int uc = compute_control_signal();
		//Sends the control signal to the hydraulic modulator.
		write_control_signal_to_hydraulic_modulator(uc);
	}
}

//Dummy function. GCC requires a main function.
int main(void) {
	return 0;
}
