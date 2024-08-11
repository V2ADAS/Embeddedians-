#include "Inc/Motion_Planing.h"
#include "../LIB/STD_TYPES.h"
#include "../MCAL/MTIMER/MTIMER_Int.h"
#include "../MCAL/MGPIO/MGPIO_int.h"
#include "../HAL/HULTRA_SONIC/HULTRA_SONIC_Int.h"
// TODO: replace math.h with implementations as needed
#include <math.h>

f32 _param_a = 0;
f32 _param_b = 0;
f32 _param_c = 0;
f32 _param_d = 0;


void Set_Param_A(f32 new_value) { _param_a = new_value; }
void Set_Param_B(f32 new_value) { _param_b = new_value; }
void Set_Param_C(f32 new_value) { _param_c = new_value; }
void Set_Param_D(f32 new_value) { _param_d = new_value; }



/*
  uses _param_a as the x-coordinate of the final point (relative to the starting point)
  uses _param_b as the y-coordinate of the final point (relative to the starting point)
    > this is expected to be negative
  uses _param_c as the maximum radius, i.e. the radius of the final circle
  uses _param_d as the angle theta (in degrees)
    > this is the angle between the positive x-axis,
    > and the radius connecting the tangential point on the final circle.
    > this is also the angle of the car with the y-axis as it enters the second circle.
 */
f32 circleLineCircle(f32 x) {
	f32 y;

#define X_F _param_a
#define Y_F _param_b
#define R_M _param_c
#define THETA (_param_d * M_PI / 180)

	// the x-coord of the second tangential point
	f32 x_2 = X_F - R_M + R_M * cos(THETA);

	// the y-coord of the second tangential point
	f32 y_2 = Y_F + R_M * sin(THETA);

	// the point at which the tangential line cuts the y-axis
	f32 c = x_2 / tan(THETA) + y_2;

	// the x-coord of the first tangential point
	f32 x_1 = - c * sin(THETA);

	// the radius of the first circle
	f32 R_1 = - c * (sin(THETA) + 1 / tan(THETA) + cos(THETA) / tan(THETA));

	if ((x >= 0) && (x < x_1)) {
		// first circle
		y = - sqrt(pow(R_1, 2) - pow(x - R_1, 2));
	} else if (x <= x_2) {
		// tangential line
		y = (x_2 - x) / tan(THETA) + Y_F + R_M * sin(THETA);
	} else if (x <= X_F) {
		// final circle
		y = sqrt(pow(R_M, 2) - pow(x - X_F + R_M, 2)) + Y_F;
	} else {
		return 1;
	}

#undef X_F
#undef Y_F
#undef R_M
#undef THETA

	return y;
}
/*
  uses _param_a as the radius of the two circles.
  uses _param_b as the x-coord of the saddle point.
 */


f32 line_path (f32 x){
	f32 y = 0.01*x*x*x ;
	return y ;
}

f32 GetSlopeOfFunc(f32 x){
	f32 deltaY = line_path(x + 0.01) - line_path(x - 0.01) ;
	f32 deltaX = 0.02 ;

	// Calculate the angle using arc tangent
	f32 angle = atan2(deltaX,deltaY);

	// Convert the angle from radians to degrees
	angle = angle * (180.0 / PI);
	return angle ;
}

f32 twoCircles(f32 x) {
	f32 y;

#define R _param_a
#define X_S _param_b

	// check if given x-coord is in the first circle
	if (x <= X_S) {
		// x-coord is on the first circle
		y = -sqrt(pow(R, 2) - pow(x - R, 2));
	} else {
		// x-coord is on the second circle

		// theta is the angle between the two centers and the vertical axis
		f32 sin_theta = 1 - X_S / R;

		// x_2 is the x-coord of the center of the second circle
		f32 x_2 = R - 2 * R * sin_theta;

		// y_2 is the y-coord of the center of the second circle
		f32 y_2 = - 2 * R * sqrt(1 - pow(sin_theta, 2));

		y = sqrt(pow(R, 2) - pow(x - x_2, 2)) + y_2;
	}

#undef R
#undef X_S

	return y;
}


//void Plot_The_Path(Scenario_t Scenario , .../*path */ );

// Parallel backward Path
f32 PaB_Path(f32 x) {
	return circleLineCircle(x);
}

// Perpendicular backward Path
f32 PeB_Path(f32 x) {
	// TODO: implement this.
	return -1;
}

// Perpendicular forward Path
f32 PeF_Path(f32 x) {
	// TODO: implement this.
	return -1;
}


Position_ST Start_Point(){
	Position_ST Position;
	Position.x=0;
	Position.y=0;
	return Position;
}

Position_ST End_Point(){
	Position_ST Position;
	Position.x= -67;
	Position.y= -150;
	return Position;
}

f32 a=0.0005,b=0;

f32 Test_Func(f32 x){
	f32 y = a * x * x * x + b ;
	return y;
}

double PEB_threeKeyPath(double x) {
    double a = 10.0; // Length of the forward straight line
    double b = 20.0; // Total length of the path
    f32 R = 63 ;
    if (x >= 0 && x < a) {

        return 0.0; // Forward straight line

    } else if (x >= a && x < a + (PI * R / 2)) {

        return sqrt(R * R - pow(x - a, 2)); // Quarter circle

    } else if (x >= a + (PI * R / 2) && x <= b) {

        return 0.0; // Backward straight line

    } else {
        return -1; // Invalid x
    }
}

double PEB_fiveKeyPath(double x) {
    double a = 10.0; // Length of the first forward straight line
    double c = 20.0; // Length of the second forward straight line
    double d = 30.0; // Total length of the path
    f32 R = 63 ;
    if (x >= 0 && x < a) {

        return 0.0; // First forward straight line

    } else if (x >= a && x < a + (PI * R / 4)) {

        return sqrt(R * R - pow(x - a, 2)); // First quarter circle

    } else if (x >= a + (PI * R / 4) && x < c) {

        return 0.0; // Second forward straight line

    } else if (x >= c && x < c + (PI * R / 2)) {

        return sqrt(R * R - pow(x - c, 2)); // Second quarter circle

    } else if (x >= c + (PI * R / 2) && x <= d) {

        return 0.0; // Backward straight line

    } else {
        return -1; // Invalid x
    }
}




