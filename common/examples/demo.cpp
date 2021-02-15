/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @date February 2021
 **/

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

