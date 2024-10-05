#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <functional>
#include <map>
// #include "../include/inja/inja.hpp"
#include <nlohmann/json.hpp>
#include <inja.hpp>
#include <ryaml/ryml_all.hpp>
using json = nlohmann::json;

using namespace std;

inja::Environment env;

class sensor;
class event_bus;

struct event {
    string name;
    list<function<void()>> callbacks;
};

class sensor {
    public:
        list<string> trigger_events;
        string name;
        string state;
        string state_template;
        string unit_of_measurement;
        // void update_state() {
        //     state = env.render(state_template, evt_bus.data);
        // }
        // void register_callback(function<void()> callback_function){
        // }

};

class event_bus {
    public:
        json data;
        list<event> events;
        map<string, sensor> sensors;
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
        void register_sensor(string entity_id, string state, string state_template, string unit, list<string> trigger_events){
            data[entity_id]["state"] = state;
            data[entity_id]["state_template"] = state_template;
            data[entity_id]["unit"] = unit;
            data[entity_id]["trigger_events"] = trigger_events;
        }

        void update_sensor_state(string entity_id){
            string mytemplate = data[entity_id]["state_template"];
            string state = env.render(mytemplate, data);
            string oldvalue;
            if (data.contains(entity_id)){
                oldvalue = data[entity_id]["state"];
            } else {
                oldvalue = "unknown";
            }
            data[entity_id]["state"] = state;
            fire_event(entity_id + " from " + oldvalue + " to " + state);
        }

        void fire_event(string name){
            cout << ("Event: " + name + "\n");
            for (const auto  

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
            if (data.contains(object_name)){
                oldvalue = data[object_name];
            } else {
                oldvalue = "unknown";
            }
            data[object_name] = new_value;
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


void my_func(){
    cout << "Hi ";
}
void my_func2(){
    cout << "Jodie";
}

string string_from_substring(c4::csubstr substring){
    std::stringstream buffer;
    buffer << substring;
    return buffer.str(); 
};

int main()  
{

    std::ifstream t("config.yaml");
    std::stringstream buffer;
    buffer << t.rdbuf();

    string my_string = buffer.str();
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(my_string));

    // c4::csubstr readval = tree["foo"].val();

    ryml::ConstNodeRef root = tree.rootref();
    ryml::ConstNodeRef templates = tree["template"];



    int myval = templates.num_children();
    
    bool my_key;
    map<string, sensor> sensors;
    evt_bus.register_event("Event 1");
    evt_bus.register_event("Event 2");
    
    for(ryml::ConstNodeRef const& yaml_sensor : templates["sensor"].children()){
        string sensor_name = string_from_substring(yaml_sensor["name"].val());
        list<string> events;
        for(ryml::ConstNodeRef const& trigger_event : yaml_sensor["trigger"].children()){
            events.push_back(string_from_substring(trigger_event["event_name"].val()));
        }
        string jinja_template = string_from_substring(yaml_sensor["state"].val());
        string unit = string_from_substring(yaml_sensor["unit"].val());

        evt_bus.register_sensor(sensor_name, "unknown", jinja_template, unit, events);
        // sensor& new_sensor = sensors[sensor_name];
        // new_sensor.name = sensor_name;
        // new_sensor.state_template = string_from_substring(yaml_sensor["state"].val());
        // for(ryml::ConstNodeRef const& trigger_event : yaml_sensor["trigger"].children()){
        //     new_sensor.trigger_events.push_back(string_from_substring(trigger_event["event_name"].val()));
        // }
        // // sensors[sensor_name] = new_sensor;
        // int i = 1;
    };

    

    // c4::csubstr readval = my_trigger[0]["platform"].val();

    sensor my_sensor;
    my_sensor.state_template = "{{ 1 }}";
    // evt_bus.register_callback("Event 1", [&my_sensor](){my_sensor.update_state();});
    evt_bus.register_callback("Event 2", my_func);
    evt_bus.register_callback("Event 2", my_func2);
    evt_bus.fire_event("Event 1");
    evt_bus.fire_event("Event 2");
    evt_bus.fire_event("Event 1");
    std::cout << evt_bus.data.dump(4) << std::endl;
   
};