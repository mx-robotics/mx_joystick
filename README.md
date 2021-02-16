# mx_joystick
The mx_joystick is a simple library to read joystick/gamepad buttons or axes with c++.<br>
__Features:__
- simple interface
- threaded
- device timeout
## Install
```
git clone https://github.com/mx-robotics/mx_joystick.git
cd mx_joystick
mkdir build
cd build
cmake ..
# If you like it somewhere else
# export INSTALL_PREFIX=/opt/mx_joystick/
# cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX .. 
# # extend the ld search path
# sudo sh -c "echo $INSTALL_PREFIX/lib > /etc/ld.so.conf.d/mx_joystick.conf"
# sudo ldconfig
# # make cmake config avaliable
# echo "export MXJoystick_DIR=$INSTALL_PREFIX/lib/CMake/MXJoystick" >> ~/.bashrc
make 
 ./common/examples/mx_demo_joystick /dev/input/js0 # for testing
sudo make install
```
## Uninstall
Be careful on this!
```
export INSTALL_PREFIX=/usr/local/ # for Ubuntu
rm $INSTALL_PREFIX/lib/CMake/MXJoystick/MXJoystick*.cmake
rm $INSTALL_PREFIX/include/mx/joystick.h
rm $INSTALL_PREFIX/lib/libmx_joystick.a
rm $INSTALL_PREFIX/bin/mx_demo_joystick
```
## Usage with cmake
Add the following statements to your CMakeLists.txt
```
...
find_package(MXJoystick REQUIRED)
...
target_link_libraries(your_target ${MXJoystick_LIBRARIES})
...

```

## example
```

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
        printf("Could not open joystick");
        return 0;
    };


    joy.start();

    mx::Joystick::Values joystick;
    for(size_t loop_count = 0; gSignalStatus == 0;  loop_count++){
        usleep(1000); 
        joy.values(joystick);
        printf("%6zu - ", joy.event_count());
        for(size_t i = 0; i < joystick.buttons.size(); i++) printf("%s %2d",i?",":"Buttons: ", joystick.buttons[i]);
        printf(" - ");
        for(size_t i = 0; i < joystick.axes.size(); i++) printf("%s %6d", i?",":"Axis: ", joystick.axes[i]);
        printf("\n");
        fflush(stdout);
    }
    
    joy.stop(); /// stops and waits for a timeout on the joy device
    

    return 0;
}


```
