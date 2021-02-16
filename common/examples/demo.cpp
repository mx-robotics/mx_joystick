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

