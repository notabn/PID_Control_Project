# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 


## Reflexions

### Parameter tunning 

#### Steering
My first try was to search for the PID parameters with an adaptation of SDG (twidle=true), but it turned out to be very tedious process . Also the sum of the squared errors that I was trying calculating was false due to the fact that the steps varied each new simulation.

I ended up varing the parameters manually. First I set the P parameter to get enought gain for the steering for the car to take the curves. Then, in order to damp oscillations I added the D term. Finally to improve the steady track error I added the I term. In this case we could have ignored the I term since there is no systematic error here.

#### Throttel
The throttle is also controlled proportional to the CTE. If the error is big the car will brake and if otherwise it will accelerate.

### Controller

After tunning the PID Controller I managed to keep the car on track, still this goes beyong one may call a plesent drive.
Due to the limitations of this controlle the car still oscillates a lot when it encounters a sharp term.
