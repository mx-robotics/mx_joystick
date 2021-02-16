/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @date February 2021
 **/
#ifndef MX_JOYSTICK_H
#define MX_JOYSTICK_H

#include <vector>
#include <future>

namespace mx {
/**
 * Joystick
 */
class Joystick {
public:

    /**
     * Current state of an axis.
     */
    class Axis {
    public:
        Axis():value(0) {}
        Axis(short value):value(value) {}
        short value;
    };

    /**
     * Current state of a button.
     */
    class Button {
    public:
        Button():value(0) {}
        Button(short value):value(value) {}
        short value;
    };

    /**
     * Constructor
     */
    Joystick();
    
    /**
     * Constructor and init
     * @param device 
     */
    Joystick(const std::string &device);
    
    /**
     * Destructor
     */
    ~Joystick();
    
    /**
     * opens device
     * @param device 
     * @return file descriptor for the joystick device, or -1 on error.
     */
    int open(const std::string &device);   
    
    /**
     * closes device
     */
    void close();    

    /**
     * Returns the number of axes on the controller or 0 if an error occurs.
     * @return number of axis
     */
    size_t get_axis_count();

    /**
     * Returns the number of buttons on the controller or 0 if an error occurs.
     * @return number of buttons
     */
    size_t get_button_count();

    /**
     * Non blocking function
     * start pulling events
     * @pre Joystick::init
     * @pre Joystick::stop
     */
    void start();
    
    /**
     * stop pulling events after the next event
     * @pre Joystick::start
     */
    void stop();

    /**
     * buttons
     * @pre Joystick::start
     * @return buttons vector
     */
    const std::vector<Button>  &buttons() const;
    /**
     * button entry
     * @pre Joystick::start
     * @param i button index
     * @return button object
     */
    const Button  &button(size_t i) const;
    
    /**
     * Axes
     * @pre Joystick::start
     * @param i axis index
     * @return axes vector
     */
    const std::vector<Axis> &axes() const;
    
    /**
     * Axis entry
     * @pre Joystick::start
     * @return axis object
     */
    const Axis &axis(size_t i) const;
    
    /**
     * event count counts button or axis events
     * @return event counter 
     */
    uint64_t event_count() const;
private:
    
    
    /**
     * blocking function to read joystick events
     * @info used by start
     * @info blockes until update_events = false
     * @return -1 on reading error otherwise 0
     */    
    int read_events();
    
    int js_;                             /// file descriptor for the joystick device, or -1 on error.
    uint64_t event_count_;               /// event count updated on every joy event
    bool update_events_;                 /// on true read_events() will block
    std::vector<Axis> axes_;             /// axes state
    std::vector<Button> buttons_;        /// buttons state
    std::future<int> future_events_;     /// future on the read_events() async with result of Joystick::read_events
};
};
#endif // MX_JOYSTICK_H
