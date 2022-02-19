# mx_joystick
The mx_joystick is a simple library to read joystick/gamepad buttons or axes with c++.<br>
__Features:__
- simple interface
- threaded
- device timeout

## Example
This a simple example how to use the mx_joystick library.
[Demo](common/examples/demo.cpp)

```cpp
#include <csignal>
#include <unistd.h>
#include <mx/joystick.h>

volatile std::sig_atomic_t gSignalStatus;
void signal_handler ( int signal ){
    gSignalStatus = signal;
}

int main(int argc, char *argv[]){
    std::signal ( SIGINT, signal_handler );
    std::string device;

    if (argc > 1) {
        device = std::string(argv[1]);
    } else {
        device = std::string("/dev/input/js0");
    }
    
    mx::Joystick joy;
    if(joy.open(device) == -1) {
        std::cout << "Could not open joystick: " << device << std::endl;
        return 0;
    };


    joy.start();

    mx::Joystick::Values joystick;
    for(size_t loop_count = 0; gSignalStatus == 0;  loop_count++){
        usleep(100000);
        joy.values(joystick);
        std::cout << std::setw( 6 ) << joystick << std::endl;
    }
    
    joy.stop(); /// stops and waits for a timeout on the joy device
    

    return 0;
}
```

## Install
### Simple
```shell
git clone https://github.com/mx-robotics/mx_joystick.git
cd mx_joystick
mkdir build
cd build
cmake ..
make 
sudo make install
```
### Installation into a spezific folder
```shell
# devine installation forder
export MXJoystick_INSTALL_PREFIX=/opt/mxr
git clone https://github.com/mx-robotics/mx_joystick.git
cd mx_joystick
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$MXJoystick_INSTALL_PREFIX .. 
# extend the ld search path
sudo sh -c "echo $MXJoystick_INSTALL_PREFIX/lib > /etc/ld.so.conf.d/mx_joystick.conf"
sudo ldconfig
# make cmake config avaliable
echo "export MXJoystick_DIR=$MXJoystick_INSTALL_PREFIX/lib/CMake" >> ~/.bashrc
make 
sudo make install
```
### Test

```shell
 ./common/examples/mx_demo_joystick /dev/input/js0 # for testing

```
## Uninstall
Be careful on this!
```shell
sudo make uninstall
```

## Usage with cmake
Add the following statements to your CMakeLists.txt
```
...
find_package(MXJoystick REQUIRED)
include_directories(${MXJoystick_INCLUDE_DIRS})
...
target_link_libraries(your_target ${MXJoystick_LIBRARIES})
...

```
