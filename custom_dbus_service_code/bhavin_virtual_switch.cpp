#include <iostream>
#include <unistd.h>
#include <dbus-1.0/dbus/dbus.h>

// Simulated State
// this will global variabel simulate a simple on/off switch
//exposed as dbus property and toggled via method call 
bool g_active = false; 

//Emit PropertiesChanges signal
//function to emit the DBus PropertiesChanged signal
// this will be called whenever 'Active' property changes.
void send_property_changed_signal(DBusConnection* conn){
    
    //create a new signal message
    DBusMessage* signal;
    signal = dbus_message_new_signal(
        "/xyz/openbmc_project/bhavin/virtualswitch/switch0", //object path
        "org.freedesktop.DBus.Properties", //Signal interface
        "PropertiesChanged" //Signal Name
    );

    //Begin appending parameters to the signal
    DBusMessageIter args;
    dbus_message_iter_init_append(signal, &args);

    //First argument : Interface whose property has changed
    const char* iface = "xyz.openbmc_project.Bhavin.VirtualSwitch.Control";
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &iface);

    //Second argument : a dictionary {"Active" -> value}
    DBusMessageIter changedProps;
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "{sv}", &changedProps);

    DBusMessageIter dictEntry;
    dbus_message_iter_open_container(&changedProps, DBUS_TYPE_DICT_ENTRY, NULL, &dictEntry);

    //Key of the dictionary: proeprty name
    const char* prop = "Active";
    dbus_message_iter_append_basic(&dictEntry, DBUS_TYPE_STRING, &prop);


    //Value: variant containing the boolean state
    DBusMessageIter variant;

    dbus_message_iter_open_container(&dictEntry, DBUS_TYPE_VARIANT, "b", &variant);
    dbus_message_iter_append_basic(&variant, DBUS_TYPE_BOOLEAN, &g_active);
    dbus_message_iter_close_container(&dictEntry, &variant);

    dbus_message_iter_close_container(&changedProps, &dictEntry);
    dbus_message_iter_close_container(&args, &changedProps);


    //Third argument : array of invalidated properties(empty here)
    DBusMessageIter empty;
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "s", &empty);
    dbus_message_iter_close_container(&args, &empty);


    //Send the signal over the bus
    dbus_connection_send(conn, signal, NULL);
    dbus_connection_flush(conn);

    //free the message
    dbus_message_unref(signal);
}


//Method handler callback function
//This handles incoming D-Bus methods calls (like Toggle)
DBusHandlerResult handle_method(DBusConnection* conn, DBusMessage* msg, void*){
    if(dbus_message_is_method_call(msg, "xyz.openbmc_project.Bhavin.VirtualSwitch.Control", "Toggle")){
        //flip the switch state (false -> true, true -> false)
        g_active = !g_active;

        //print the new state to stdout (for debugging)
        std::cout<<"Switched state: "<<( g_active ? "ON":"OFF") <<std::endl;

        //send the PropertiesChanged signal
        send_property_changed_signal(conn);

        //Send back an empty method return reply to the caller
        DBusMessage* reply = dbus_message_new_method_return(msg);
        dbus_connection_send(conn, reply, NULL);
        dbus_connection_flush(conn);
        dbus_message_unref(reply);

        return DBUS_HANDLER_RESULT_HANDLED;
    }

    // if not handled method, let others handle it
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main(){
    DBusError err; // for storing DBus error details
    DBusConnection* conn; //DBus connection object

    dbus_error_init(&err); //initialize error variable

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if(dbus_error_is_set(&err)){
        std::cerr<<"Connection error: "<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }


    //Request a well known service name on the bus

    dbus_bus_request_name(conn, "xyz.openbmc_project.Bhavin.VirtualSwitchService", //our unique service name
    DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    if(dbus_error_is_set(&err)){
        std::cerr<<"Name error: "<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }

    //Register the object path to received method calls
    DBusObjectPathVTable vtable = {
        nullptr, //no unregister callback
        handle_method, //Method call handler
        nullptr //no introspection handler (could be added later)
    };

    dbus_connection_register_object_path(conn, "/xyz/openbmc_project/bhavin/virtualswitch/switch0", &vtable, nullptr);

    //Print status to console
    std::cout<<"Bhavin's virtual Switch D-Bus Service is now running...."<<std::endl;


    //Main loop: block and wait for incoming method calls
    while(true){
        //Block until a message is received and then dispatches it
        dbus_connection_read_write_dispatch(conn, -1);
    }

    return 0;
}

