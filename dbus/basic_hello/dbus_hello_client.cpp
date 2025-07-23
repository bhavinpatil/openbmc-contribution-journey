#include<dbus-1.0/dbus/dbus.h>
#include<cstring>
#include<iostream>

int main(){
    DBusError err;
    DBusMessage* msg;
    DBusConnection* conn;
    DBusMessageIter args;
    DBusMessage* reply;
    char* reply_str;

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(dbus_error_is_set(&err)){
        std::cerr<<"client::connection error - "<<err.message<<std::endl;
        dbus_error_free(&err);
    }

    if(!conn) return 1;

    msg = dbus_message_new_method_call(
        "com.example.HelloService", //target service
        "/com/example/HelloService", //object path
        "com.example.HelloInterface", //interface
        "SayHello" //method name
    );

    if(!msg){
        std::cerr<<"client::message null"<<std::endl;
        return 1;
    }

    std::cout<<"Enter your name: "<<std::endl;
    std::string input;
    std::cin>>input;

    const char* name = input.c_str();
    dbus_message_iter_init_append(msg, &args);
    if(!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name)){
        std::cerr<<"client::out of memory!"<<std::endl;
        return 1;
    }


    //send message and get reply

    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
    dbus_message_unref(msg);

    if(dbus_error_is_set(&err)){
        std::cerr<<"client::error in reply - "<<err.message<<std::endl;
        dbus_error_free(&err);
        return 1;
    }

    //read reply
    if(!dbus_message_iter_init(reply, &args)){
        std::cerr<<"client::reply has no argument!"<<std::endl;
        return 1;
    }

    if(DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)){
        std::cerr<<"client::wrong return type!"<<std::endl;
        return 1;
    }

    dbus_message_iter_get_basic(&args, &reply_str);
    std::cout<<"client::received reply - "<<reply_str<<std::endl;

    dbus_message_unref(reply);
    return 0;
}