#include<dbus-1.0/dbus/dbus.h>
#include<iostream>
#include<cstring>
#include<unistd.h>

//Simulate LED state(property) - values like "ON", "OFF" and "BLINKING"

std::string g_led_state = "OFF";

const char* OBJECT_PATH = "/xyz/openbmc_project/led/led0";        //openBMC style object path
const char* INTERFACE_NAME = "xyz.openbmc_project.Led.Control";   //interface represnting LED Control
const char* SERVICE_NAME = "xyz.openbmc_project.LEDService";      //Our DBus service name

//function to emit singal when LED state changes

void emit_led_state_changed_signal(DBusConnection* conn, const char* new_state){
    DBusMessage* signal;
    DBusMessageIter args;

    //Create D-Bus signal on our interface
    signal = dbus_message_new_signal(OBJECT_PATH, INTERFACE_NAME, 
        "LEDStateChanged" // signal name
    );

    dbus_message_iter_init_append(signal, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &new_state);

    dbus_connection_send(conn, signal, NULL);
    dbus_connection_flush(conn);
    dbus_message_unref(signal);

    std::cout<<"LED SERVICE::emitted signal - LEDStateChanged to "<<new_state<<std::endl;
}

// function to handle standard DBus Get request

DBusMessage* handle_property_get(DBusMessage* msg){
    DBusMessage* reply;
    DBusMessageIter reply_iter, varient_iter;

    const char* iface;
    const char* prop;

    DBusError err;
    dbus_error_init(&err);

    // extract interface name and property name from incoming message

    if(!dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &iface, DBUS_TYPE_STRING, &prop, DBUS_TYPE_INVALID)){
        std::cerr<<"LED SERVICE::error getting property arguments:"<<err.message<<std::endl;
        dbus_error_free(&err);
        return NULL;
    }

    std::cout<<"LED SERVICE:: Get request for property - "<<prop<<std::endl;

    //wrap the g_led_state in a variant and send it as reply

    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &reply_iter);
    dbus_message_iter_open_container(&reply_iter, DBUS_TYPE_VARIANT, "s", &varient_iter);

    const char* value = g_led_state.c_str();

    dbus_message_iter_append_basic(&varient_iter, DBUS_TYPE_STRING, &value);

    dbus_message_iter_close_container(&reply_iter, &varient_iter);
    return reply;
}

//functio to handel standard DBus Set request

DBusMessage* handle_property_set(DBusMessage* msg, DBusConnection* conn){
    DBusMessage* reply;
    DBusMessageIter msg_iter, varient_iter;

    const char* iface;
    const char* prop;

    char* new_value;

    //extract : interface name, property name and the variant value
    dbus_message_iter_init(msg, &msg_iter);  // main msg, then loading arguments in msg_iter
    dbus_message_iter_get_basic(&msg_iter, &iface);

    dbus_message_iter_next(&msg_iter);
    dbus_message_iter_get_basic(&msg_iter, &prop);

    dbus_message_iter_next(&msg_iter);
    dbus_message_iter_recurse(&msg_iter, &varient_iter);

    dbus_message_iter_get_basic(&varient_iter, &new_value);

    std::cout<<"LED SERVICE:: Set request for property - "<<prop<<" with value - "<<new_value<<std::endl;
    
    //update the property value
    g_led_state = new_value;

    //emit the signal to others to know LED state has changed

    emit_led_state_changed_signal(conn, g_led_state.c_str());

    reply = dbus_message_new_method_return(msg);
    return reply;
}


int main(){
    DBusError err;
    DBusConnection* conn;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    //register the service name on the Dbus

    dbus_bus_request_name(conn, SERVICE_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    std::cout<<"LED SERVICE:: Running on: "<<OBJECT_PATH<<std::endl;
    std::cout<<"LED SERVICE:: Initial State - "<<g_led_state<<std::endl;

    //main event loop to handle property Get/Set calls

    while(true){
        dbus_connection_read_write(conn, 0);

        DBusMessage* msg = dbus_connection_pop_message(conn);

        if(!msg){
            usleep(100000);
            continue;
        }

        if(dbus_message_is_method_call(msg, "org.freedesktop.DBus.Properties", "Get")){
            DBusMessage* reply = handle_property_get(msg);
            if(reply){
                dbus_connection_send(conn, reply, NULL);
                dbus_connection_flush(conn);
                dbus_message_unref(reply);
            }
        }else if(dbus_message_is_method_call(msg, "org.freedesktop.DBus.Properties", "Set")){
            DBusMessage* reply = handle_property_set(msg, conn);
            if(reply){
                dbus_connection_send(conn, reply, NULL);
                dbus_connection_flush(conn);
                dbus_message_unref(reply);
            }
        }

        dbus_message_unref(msg);
    }
    return 0;

}
