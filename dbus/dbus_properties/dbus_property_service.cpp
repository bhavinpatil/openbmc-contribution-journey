#include<iostream>
#include<dbus-1.0/dbus/dbus.h>
#include<unistd.h>
#include<cstring>


std::string g_status = "OK"; // this will be the property we will be exposing



//function to handle standard org.freedesktop.DBus.Properties.Get method
DBusMessage* handle_property_get(DBusMessage* msg){
    DBusMessage* reply;
    DBusMessageIter reply_iter, varient_iter;

    const char* iface;
    const char* prop;

    DBusError err;

    dbus_error_init(&err);

    if(!dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &iface, DBUS_TYPE_STRING, &prop, DBUS_TYPE_INVALID)){
        std::cerr<<"Error getting argyuments: "<<err.message<<std::endl;
        dbus_error_free(&err);
        return nullptr;
    }

    std::cout<<"service::get request for property - "<<prop<<std::endl;


    //creating reply message with string varient
    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &reply_iter);

    dbus_message_iter_open_container(&reply_iter, DBUS_TYPE_VARIANT, "s", &varient_iter);

    const char* status = g_status.c_str();

    dbus_message_iter_append_basic(&varient_iter, DBUS_TYPE_STRING, &status);

    dbus_message_iter_close_container(&reply_iter, &varient_iter);

    return reply;
}


int main(){
    DBusError err;
    DBusConnection* conn;


    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    dbus_bus_request_name(conn, "com.example.PropertyService", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    std::cout<<"service::exposed property - status = " <<g_status<<std::endl;


    while(true){
        dbus_connection_read_write(conn, 0);

        DBusMessage* msg = dbus_connection_pop_message(conn);

        if(!msg){
            usleep(100000);
            continue;
        }

        //only respone to standard GET call on property

        if(dbus_message_is_method_call(msg, "org.freedesktop.DBus.Properties", "Get")){
            DBusMessage* reply = handle_property_get(msg);
            if(reply){
                dbus_connection_send(conn, reply, NULL);
                dbus_message_unref(reply);
            }
        }

        dbus_message_unref(msg);
    }
    return 0;
}
