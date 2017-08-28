#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

using namespace std;

// for convenience
using json = nlohmann::json;


//search pid parameters
bool fit_param = true;

//reset msg
std::string reset_msg = "42[\"reset\",{}]";

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
    auto found_null = s.find("null");
    auto b1 = s.find_first_of("[");
    auto b2 = s.find_last_of("]");
    if (found_null != std::string::npos) {
        return "";
    }
    else if (b1 != std::string::npos && b2 != std::string::npos) {
        return s.substr(b1, b2 - b1 + 1);
    }
    return "";
}


void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws){
    std::cout << "Reset Simulator" << std::endl;
    std::string msg("42[\"reset\",{}]");
    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

bool twidle_Kp = false;
bool twidle_Ki=false;
bool twidle_Kd= false;



int main()
{
    uWS::Hub h;
    
    PID pid;
    
    
    double Kp = 2;
    double Kd = 4;
    double Ki = 0.0001;
    
    pid.Init(Kp, Kd, Ki);
    
    
    std::vector<double> dp;
    dp.assign(3,0.1);
    
    double val[] = {Kp,Kd,Ki};
    std::vector<double> p (val, val + sizeof(val) / sizeof(double)) ;
    
    
    
    double best_err ;
    bool reset = false;
    int i = 0;
    bool twidle = false;
    int pos = 0;
    double sum_dp =0 ;
    
    h.onMessage([&pid,&i,&dp,&p,&best_err,&reset,&twidle,&pos,&sum_dp](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
        // "42" at the start of the message means there's a websocket message event.
        // The 4 signifies a websocket message
        // The 2 signifies a websocket event
        if (length && length > 2 && data[0] == '4' && data[1] == '2')
        {
            auto s = hasData(std::string(data).substr(0, length));
            if (s != "") {
                auto j = json::parse(s);
                std::string event = j[0].get<std::string>();
                if (event == "telemetry") {
                    // j[1] is the data JSON object
                    double cte = std::stod(j[1]["cte"].get<std::string>());
                    double speed = std::stod(j[1]["speed"].get<std::string>());
                    double angle = std::stod(j[1]["steering_angle"].get<std::string>());
                    double steer_value;

                    if  (i == 0){
                        best_err = pid.squared_err;
                    }
                    
                    pid.UpdateError(cte);
                    steer_value =  pid.TotalError();
                    /*if (twidle){
                        p[pos] += dp[pos];
                        pid.Init(p[0],p[1],p[2]);
                        cout<<"p_pos"<<pos<<" "<< p[pos]<<endl;
                        //cout<<"i is"<<i<<endl;
                        //cout<<"pos is " <<pos<<endl;
                        cout<<"pid "<<pid.Kp<<" "<<pid.Kd<<" "<<pid.Ki<<endl;
                        //cout<<"dp "<<dp[0]<<" "<<dp[1]<<" "<<dp[2]<<endl;
                        if (pid.squared_err < best_err){
                            best_err = pid.squared_err;
                            dp[pos] *= 1.1;
                            if  (pos < 1){
                                pos = pos + 1;
                                i = 0;
                            }else{
                                pos = 0 ;
                                cout <<"reset to 0"<<endl;
                            }
                        }else{
                            if (reset){
                                reset = false;
                                cout<<"compensate"<<endl;
                                p[pos] += 2*dp[pos];
                                dp[pos] *= 0.9;
                                if  (pos < 1){
                                    pos = pos + 1;
                                    i = 0;
                                }else{
                                    pos = 0;
                                    cout <<"reset to 0"<<endl;
                                }
                            }else{
                                cout<<"decreasing"<<endl;
                                p[pos] -= 3*dp[pos];
                                reset = true;
                            }
                        }
                        // cout<<"dp "<<dp[0]<<" "<<dp[1]<<" "<<dp[2]<<endl;
                        
                        double prev_sum = sum_dp;
                        sum_dp = pid.Kp +pid.Kp+pid.Ki;
                        
                        /*
                        if(fabs(prev_sum-sum_dp)<0.000000001){
                            i = 0;
                            reset_simulator(ws);
                            pos = 0;
                            dp.assign(3,0.1);
                        }
                        
                        
                         if ((i > 500) && (sum_dp < 0.2) && twidle){
                         reset_simulator(ws);
                         pos = 0 ;
                         
                         }
                         if (sum_dp >5){
                         twidle = false;
                         
                         }
                        
                        //cout<<"fasb"<<fabs(cte)<<endl;
                        //cout<<"twiddle"<<twidle<<endl;
                       
                        if ( fabs(cte)>2.2 ) {
                            reset_simulator(ws);
                            pos = 0;
                            dp.assign(3,0.1);
                        }
                       
                    }
                    */
 
                    
                    //steer_value = std::max(std::min(-1.5, steer_value), 1.5);
                    

                    
                    double targetSpeed = fabs(0.1 *(1.-steer_value) + 0.10);
                    
                    std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;
                    
                    json msgJson;
                    msgJson["steering_angle"] = steer_value;
                    msgJson["throttle"] = 0.2;
                    //msgJson["throttle"] = targetSpeed;
                    auto msg = "42[\"steer\"," + msgJson.dump() + "]";
                    //std::cout << msg << std::endl;
                    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                }
            }
        }
        
        
    });
    
    
    
    
    
    
    
    
    
    // We don't need this since we're not using HTTP but if it's removed the program
    // doesn't compile :-(
    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
        const std::string s = "<h1>Hello world!</h1>";
        if (req.getUrl().valueLength == 1)
        {
            res->end(s.data(), s.length());
        }
        else
        {
            // i guess this should be done more gracefully?
            res->end(nullptr, 0);
        }
    });
    
    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });
    
    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });
    
    int port = 4567;
    if (h.listen(port))
    {
        std::cout << "Listening to port " << port << std::endl;
    }
    else
    {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }
    h.run();
}
