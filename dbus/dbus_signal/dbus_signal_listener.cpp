#include<csignal>
#include<dbus-1.0/dbus/dbus.h>
#include<iostream>
#include<unistd.h>

int main(){
    DBusConnection* conn;
    DBusError err;

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err); //connecting to the session bus

    //adding the matching rule for our specific dbus signal

    dbus_bus_add_match(conn, "type='signal', interface='com.example.SignalEmitter', member='Tick'", &err);

    dbus_connection_flush(conn);

    std::cout<<"listener::tick signals..."<<std::endl;

    while(true){
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);

        if(!msg){
            usleep(100000); //wait 100ms
            continue;
        }

        if(dbus_message_is_signal(msg, "com.example.SignalEmitter", "Tick")){
            DBusMessageIter args;
            int tick_value;
            char* sender_msg;

            if(dbus_message_iter_init(msg, &args)){
                dbus_message_iter_get_basic(&args, &sender_msg);
                dbus_message_iter_next(&args);
                dbus_message_iter_get_basic(&args, &tick_value);
                std::cout<<"listener::received tick signal - "<<tick_value<<" from "<<sender_msg<<std::endl;
            }
        }
        dbus_message_unref(msg);
    }
    return 0;
}