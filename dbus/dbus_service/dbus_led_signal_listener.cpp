#include<dbus-1.0/dbus/dbus.h>
#include<iostream>
#include<cstring>
#include<unistd.h>

int main(){
    DBusError err;
    dbus_error_init(&err);

    DBusConnection* conn;

    // Step:1 Connect to Bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if(dbus_error_is_set(&err)){
        std::cerr<<"lister::Connection err: "<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }


    // Step 2: Add a match rule to receive only our desired signal
    const char* match_rule = "type='signal', interface='xyz.openbmc_project.Led.Control', member='LEDStateChanged'";
    // "type='signal',interface='xyz.openbmc_project.Led.Control',member='LEDStateChanged'";

    dbus_bus_add_match(conn, match_rule, &err);

    dbus_connection_flush(conn);

    if(dbus_error_is_set(&err)){
        std::cerr<<"lister::Match Error: "<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }


    std::cout<<"lister::Waiting for LEDStateChanged Signal...."<<std::endl;


    // Step 3: Message loop to wait for signal
    while(true){
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);

        if(!msg){
            usleep(100000);
            continue;
        }

        // Step 4: Is this the signal we expect?
        if(dbus_message_is_signal(msg, "xyz.openbmc_project.Led.Control", "LEDStateChanged")){
            DBusMessageIter args;
            char* state;

            //Initialize iterator
            if(!dbus_message_iter_init(msg, &args)){
                std::cerr<<"lister::Message has no arguments!"<<std::endl;
            }else if(DBUS_TYPE_STRING!=dbus_message_iter_get_arg_type(&args)){
                std::cerr<<"lister::argument is not string!"<<std::endl;
            }else{
                dbus_message_iter_get_basic(&args, &state);
                std::cout<<"lister::LED State Changed to: "<< state <<std::endl;
            }
        }
        dbus_message_unref(msg);
    }

    return 0;

}