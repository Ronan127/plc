#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <functional>

using namespace std;

struct event {
    string name;
    list<std::function<void()>> callbacks;
};

class event_bus {
    public:
        list<event> events;
        void register_event(string name) {
            event my_event;
            my_event.name = name;
            events.push_back(my_event);
        }
        void register_callback(string name, std::function<void()> callback_function){
            for (event &myevent : events){
                if (myevent.name == name){
                    myevent.callbacks.push_back(callback_function);
                    break;
                };
            };
        }
        void fire_event(string name){
            for (event &myevent : events){
                if (myevent.name == name){
                    for (std::function<void()> &my_function : myevent.callbacks){
                        my_function();
                    }
                    break;
                };
            };
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

void my_func(){
    cout << "Hi ";
}
void my_func2(){
    cout << "Jodie";
}

int main()
{
   event_bus evt_bus;

   evt_bus.register_event("Event 1");
   evt_bus.register_callback("Event 1", my_func);
   evt_bus.register_event("Event 2");
   evt_bus.register_callback("Event 2", my_func);
   evt_bus.register_callback("Event 2", my_func2);
   evt_bus.fire_event("Event 1");
   evt_bus.fire_event("Event 2");
   
}