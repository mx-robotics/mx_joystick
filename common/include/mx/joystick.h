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
     * Joystick values
     */
    class Values {
    public:
        std::vector<short> axes;           /// axes state
        std::vector<short> buttons;        /// buttons state
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
     * joystick state
     * @pre Joystick::start
     * @return buttons and axis values
     */
    Values values() const;
    
    /**
     * joystick state
     * @pre Joystick::start
     * @param des destination to copy the values to
     * @return buttons and axis values
     */
    Values &values(Values &des) const;
    
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
    Values values_;                      /// joystick state
    std::future<int> future_events_;     /// future on the read_events() async with result of Joystick::read_events
};
};
#endif // MX_JOYSTICK_H
