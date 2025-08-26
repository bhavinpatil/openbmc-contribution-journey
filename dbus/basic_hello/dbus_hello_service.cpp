// #include<dbus/dbus.h>
#include<dbus-1.0/dbus/dbus.h> //core dbus functions and types
#include<iostream> //for console output
#include<unistd.h> 
#include<cstring> // for c string functions

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn){
    DBusMessage* reply;
    DBusMessageIter args;

    bool success;

    const char* reply_str;

    //extract argument
    char* name = nullptr;

    if(!dbus_message_iter_init(msg, &args)){
        std::cout<<"Message has no arguments"<<std::endl;
        return;
    }

    if(DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)){
        std::cout<<"Argument is not string!"<<std::endl;
        return;
    }

    dbus_message_iter_get_basic(&args, &name);
    std::cout<<"service::received - SayHello with name - "<<name<<std::endl;

    std::string result = "Hello, ";
    result += name;


    //creating reply
    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &args);

    reply_str = result.c_str();

    success = dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &reply_str);

    if(!success){
        std::cerr<<"service::out of memory!"<<std::endl;
        exit(1);
    }

    //send reply
    dbus_connection_send(conn, reply, NULL);
    dbus_message_unref(reply);
}

int main(){
    DBusError err;
    DBusConnection* conn;

    int ret;

    dbus_error_init(&err);

    //connect to system bus

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(dbus_error_is_set(&err)){
        std::cerr<<"service::connection error: "<<err.message<<std::endl;
        dbus_error_free(&err);
    }

    if(!conn) return 1;


    //request a well known name on bus

    ret = dbus_bus_request_name(conn, "com.example.HelloService", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if(dbus_error_is_set(&err)){
        std::cerr<<"service:: name error - "<<err.message<<std::endl;
        dbus_error_free(&err);
    }

    if(ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) return 1;

    std::cout<<"service::service is running...."<<std::endl;

    while(true){
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if(msg == NULL){
            usleep(100000);
            continue;
        }

        //check if it's expected method call

        if(dbus_message_is_method_call(msg, "com.example.HelloInterface", "SayHello")){
            reply_to_method_call(msg, conn);
        }

        dbus_message_unref(msg);
    }

    return 0;
}
