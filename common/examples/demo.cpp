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

