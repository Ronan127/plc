#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <functional>
// #include "../include/inja/inja.hpp"
#include <nlohmann/json.hpp>
#include <inja.hpp>
using json = nlohmann::json;

using namespace std;

inja::Environment env;


struct event {
    string name;
    list<function<void()>> callbacks;
};

class event_bus {
    public:
        json data;
        list<event> events;
        void register_event(string name) {
            event my_event;
            my_event.name = name;
            events.push_back(my_event);
        }
        void register_callback(string name, function<void()> callback_function){
            for (event &myevent : events){
                if (myevent.name == name){
                    myevent.callbacks.push_back(callback_function);
                    break;
                };
            };
        }
        void fire_event(string name){
            cout << ("Event: " + name + "\n");
            for (event &myevent : events){
                if (myevent.name == name){
                    for (function<void()> &my_function : myevent.callbacks){
                        my_function();
                    }
                    break;
                };
            };
        }
        void update_state(string object_name, string new_value){
            string oldvalue;
            if (data.contains("object_name")){
                oldvalue = data[object_name];
            } else {
                oldvalue = "unknown";
            }
            fire_event(object_name + " from " + oldvalue + " to " + new_value);
        }
    private:
        // event &active_event;
        // void get_event(string name){
        //     int i = 0;
        //     for (event myevent : events){
        //         if (myevent.name == name){
        //             active_event = myevent;
        //         };
        //         i++;
        //     };
        // }
};

event_bus evt_bus;

class sensor {
    public:
        string trigger_event;
        string name;
        string state;
        string state_template;
        string unit_of_measurement;
        sensor(string sensor_name){
            name = sensor_name;
        }
        void update_state() {
            state = env.render(state_template, evt_bus.data);
            evt_bus.update_state(name, state);
        }

};

void my_func(){
    cout << "Hi ";
}
void my_func2(){
    cout << "Jodie";
}

int main()
{

    sensor my_sensor("my_sensor");
    my_sensor.state_template = "{{ 1 }}";
    evt_bus.register_event("Event 1");
    evt_bus.register_callback("Event 1", [&my_sensor](){my_sensor.update_state();});
    evt_bus.register_event("Event 2");
    evt_bus.register_callback("Event 2", my_func);
    evt_bus.register_callback("Event 2", my_func2);
    evt_bus.fire_event("Event 1");
    evt_bus.fire_event("Event 2");
   
}