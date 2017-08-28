#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double p, double d, double i) {
    
    Kp = p;
    Ki = i;
    Kd = d;
    p_error= 0;
    i_error = 0;
    d_error=0;
    squared_err = 0;
    

}

void PID::UpdateError(double cte) {
    d_error = cte-p_error;
    p_error = cte;
    i_error += cte;
    squared_err += cte*cte;
}

double PID::TotalError() {
    double steeringAngle = -Kp*p_error -Kd*d_error - Ki*i_error;
    /*steeringAngle = steeringAngle<-1?-1:steeringAngle;
    steeringAngle = steeringAngle>1?1:steeringAngle;*/
    return steeringAngle;
    
}


