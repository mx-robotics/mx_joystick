/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @date February 2021
 **/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <future>
#include <mx/joystick.h>

using namespace mx;

Joystick::Joystick() : js_(-1), update_events_(false) {
}

Joystick::Joystick(const std::string &device) : js_(-1), update_events_(false) {
    open(device);
}

int Joystick::open(const std::string &device) {
    js_ = ::open(device.c_str(), O_RDONLY);
    if (js_ == -1) {
        return js_;
    }

    axes_.resize(get_axis_count());
    buttons_.resize(get_button_count());
    return js_;
}


void Joystick::close() {
    if(js_ != -1)  ::close(js_);
}

Joystick::~Joystick() {
    close();
}

size_t Joystick::get_axis_count(){
    uint8_t nr_of_axes;
    if (ioctl(js_, JSIOCGAXES, &nr_of_axes) == -1) return 0;
    return nr_of_axes;
}

size_t Joystick::get_button_count(){
    uint8_t nr_of_buttons;
    if (ioctl(js_, JSIOCGBUTTONS, &nr_of_buttons) == -1)  return 0;
    return nr_of_buttons;
}



int Joystick::read_events() {
    struct js_event event;
    size_t axis;

    while (update_events_)
    {
        size_t bytes = read(js_, &event, sizeof(event));
        if (bytes != sizeof(event))  return -1;
        switch (event.type)
        {
        case JS_EVENT_BUTTON:
            buttons_[event.number] = event.value;
            break;
        case JS_EVENT_AXIS:
        {
            uint8_t axis = event.number / 2;
            if (axis < axes_.size()) {
                if (event.number % 2 == 0)
                    axes_[axis].x = event.value;
                else
                    axes_[axis].y = event.value;
            }
        }
        break;
        }
    }
    return 0;
}

void Joystick::start() {
    update_events_ = true;
    future_events_ = std::async(std::launch::async, &Joystick::read_events, this);
}
void Joystick::stop() {
    update_events_ = false;
}

const std::vector<Joystick::Button>  &Joystick::buttons() const {
    return buttons_;
}
const Joystick::Button  &Joystick::button(size_t i) const {
    return buttons_[i];
}
const std::vector<Joystick::Axis> &Joystick::axes() const {
    return axes_;
}
const Joystick::Axis &Joystick::axis(size_t i) const {
    return axes_[i];
}

