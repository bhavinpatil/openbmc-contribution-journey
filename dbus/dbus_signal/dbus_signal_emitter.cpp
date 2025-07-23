#include<dbus-1.0/dbus/dbus.h>
#include<cstring>
#include<iostream>
#include<unistd.h>


// this main service side will emits the signal every 2 seconds
int main(){
    DBusError err;
    DBusConnection* conn;

    dbus_error_init(&err);


    //connecti to session bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(dbus_error_is_set(&err)){
        std::cerr<<"emitter::connection error!"<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }


    //request a name on bus
    dbus_bus_request_name(conn, "com.example.SignalEmitter", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    int tick = 0;
    const char* sender = "Signal from Bhavin!";

    while(true){
        DBusMessage* msg;
        DBusMessageIter args;

        //creating new signal message
        msg = dbus_message_new_signal(
            "/com/example/SignalEmitter", //object path
            "com.example.SignalEmitter", //interface
            "Tick" //signal name
        );

        //appending integer argument to signal 
        dbus_message_iter_init_append(msg, &args);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &sender);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &tick);


        //send the signal
        dbus_connection_send(conn, msg, NULL);
        dbus_connection_flush(conn); // ensuring it is send immediately

        std::cout<<"emitter::signal send tick - "<<tick<<std::endl;

        dbus_message_unref(msg); //free memory
        tick++;
        sleep(2); // wait before sending next signal 
    }

    return 0;
}
