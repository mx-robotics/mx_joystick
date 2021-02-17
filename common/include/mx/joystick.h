/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @date February 2021
 **/
#ifndef MX_JOYSTICK_H
#define MX_JOYSTICK_H

#include <vector>
#include <future>
#include <mutex>
#include <iostream>
#include <iomanip>

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
        Values(): event_id(0){
        }
        Values(const Values &v)
        : event_id(v.event_id), buttons(v.buttons), axes(v.axes) {
        }
        uint64_t event_id;                   /// event id
        std::vector<short> buttons;          /// buttons state
        std::vector<short> axes;             /// axes state
        float axis(size_t i) const {
            return ((float) axes[i]) / ((float) std::numeric_limits<short>::max());
        }
        bool button(size_t i) const {
            return (buttons[i] > 0);
        }
        friend std::ostream &operator << ( std::ostream &os, const Values &o ) {
            os << "event: " << std::setw( 6 ) << o.event_id;
            for(size_t i = 0; i < o.buttons.size(); i++) os << (i?", ":"; buttons: ") << o.button(i);
            for(size_t i = 0; i < o.axes.size(); i++) {
                os << (i?", ":"; axes: ") << std::fixed << std::setw(4) << std::setprecision(2) << o.axis(i);
            }
            return os;
        };
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
     * joystick state and a return value if a button was pressed since last time getting values
     * @pre Joystick::start
     * @param des destination to copy the values. The evnet id is used to compute the event differnce on the return
     * @return number of events since last time getting data to des
     */
    int get(Values &des) const;
    
    /**
     * future on the event checking thread
     * useful to check if stop was successfull
     * @pre start, stop
     * @return future
     */
    const std::future<int> &get_future();
private:
    
    
    /**
     * blocking function to read joystick events
     * @info used by start
     * @info blockes until update_events = false
     * @return -1 on reading error otherwise 0
     */    
    int read_events();
    
    int js_;                             /// file descriptor for the joystick device, or -1 on error.
    bool update_events_;                 /// on true read_events() will block
    Values values_;                      /// joystick state
    std::future<int> future_events_;     /// future on the read_events() async with result of Joystick::read_events
    mutable std::mutex mutex_values_;    /// mutex on values
};
};
#endif // MX_JOYSTICK_H
