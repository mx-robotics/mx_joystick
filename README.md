# mx_joystick
This is a very simple class to access joystick/gamepad buttons or axes for c++.
## install
```
mkdir build
cd build
make 
sudo make install
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
#include <unistd.h>
#include <mx/joystick.h>

int main(int argc, char *argv[])
{
    std::string device;

    if (argc > 1) {
        device = std::string(argv[1]);
    } else {
        device = std::string("/dev/input/js0");
    }
    mx::Joystick joy;
    if(joy.init(device) == -1) {
        printf("Could not open joystick");
        return 0;
    };


    /* This loop will exit if the controller is unplugged. */
    joy.start();

    while(true) {
        usleep(1000);
        printf("Buttons: ");
        for(size_t i = 0; i < joy.buttons().size(); i++) printf("%s %zu: %2d",i?",":"", i, joy.button(i).value);
        printf("\nAxis: ");
        for(size_t i = 0; i < joy.axes().size(); i++) printf("%s %zu: (%6d, %6d)", i?",":"", i, joy.axis(i).x, joy.axis(i).y);
        printf("\n");
        fflush(stdout);
    }

    return 0;
}
```
