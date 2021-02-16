# mx_joystick
This is a very simple class to access joystick/gamepad buttons or axes for c++.
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
## usage with cmake
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
/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @date 15th of February 2021
 **/

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

    for(size_t loop_count = 0; gSignalStatus == 0;  loop_count++){
        usleep(1000);
        printf("%6zu Buttons: ", loop_count);
        for(size_t i = 0; i < joy.buttons().size(); i++) printf("%s %2d",i?",":"", joy.button(i).value);
        printf("; Axis: ");
        for(size_t i = 0; i < joy.axes().size(); i++) printf("%s (%6d, %6d)", i?",":"", joy.axis(i).x, joy.axis(i).y);
        printf("\n");
        fflush(stdout);
    }
    
    joy.stop(); 
    

    return 0;
}
```
