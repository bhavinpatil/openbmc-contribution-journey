// main.cpp

// Minimal manual DBus service using low-level libdbus (C API) wrapped in C++

// Service: xyz.openbmc_project.Bhavin.Demo
// Object Path: /xyz/openbmc_project/bhavin/demo
// Interface: xyz.openbmc_project.Bhavin.Demo.Interface

// Build: see Makefile

#include <dbus-1.0/dbus/dbus.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <signal.h>

// ================ Service Data (properties) ================
static std::string g_demoString = "HelloWolrd";
static int32_t g_demoCounter = 0;

// ================ Introspection XML ================
// describe our interface, properties, methods, signals.
// This string is returned by the introspect method of org.freedesktop.DBus.Introspectable.

static const char *introspection_xml =
    R"XML(<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN)" "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">
<node>
    <interface name="xyz.openbmc_project.Bhavin.Demo.Interface">
        <method name="IncrementCounter"/>
        <method name="ResetCounter"/>
        <signal name="CounterChanged">
            <arg name="newValue" type="i"/>
        </signal>
        <property name="DemoString" type="s" access="readwrite">
            <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
        </property>
        <property name="DemoCounter" type="i" access="readwrite">
            <annotation name="org.freedesktop.DBus.Property.EmitsChangedSignal" value="true"/>
        </property>
    </interface>

    <interface name="org.freedesktop.DBus.Introspectable">
        <method name="Introspect">
            <arg type="s" direction="out"/>
        </method>
    </interface>

    <interface name="org.freedesktop.DBus.Properties">
        <method name="Get">
            <arg name="interface_name" type="s" direction="in"/>
            <arg name="prop_name" type="s" direction="in"/>
            <arg name="value" type="v" direction="out"/>
        </method>
        <method name="Set">
            <arg name="interface_name" type="s" direction="in"/>
            <arg name="prop_name" type="s" direction="in"/>
            <arg name="value" type="v" direction="in"/>
        </method>
        <method name="GetAll">
            <arg name="interface_name" type="s" direction="in"/>
            <arg name="props" type="a{sv}" direction="out"/>
        </method>
        <signal name="PropertiesChanged">
            <arg name="interface_name" type="s"/>
            <arg name="changed_props" type="a{sv}"/>
            <arg name="invalidated_props" type="as"/>
        </signal>
    </interface>
</node>
)XML";

// ================ Emit Signal Helper Function ================
// Helper: emit org.freedesktop.DBus.Properties.PropertyChanged signal with 1 property changed

// prop_type_signature = {"s", "i"}

static void emit_properties_changed(DBusConnection *conn, const char *interface_name, const char *prop_name, const char *prop_type_signature, const void *prop_value_ptr)
{

    DBusMessage *msg = dbus_message_new_signal("/xyz/openbmc_project/bhavin/demo", "org.freedesktop.DBus.Properties", "PropertiesChanged");

    if (!msg)
    {
        std::cerr << "Failed to create PropertiesChanged message!" << std::endl;
        return;
    }

    DBusMessageIter iter;
    dbus_message_iter_init_append(msg, &iter);

    // Append interface string
    const char *iface_c = interface_name;
    if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &iface_c))
    {
        std::cerr << "Failed to append interface name!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    // Append a{sv} map of changed properties
    DBusMessageIter array_iter;
    if (!dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &array_iter))
    {
        std::cerr << "Failed to open array container!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    // one dict entry: {propname => variant(value)}
    DBusMessageIter dict_entry;
    if (!dbus_message_iter_open_container(&array_iter, DBUS_TYPE_DICT_ENTRY, NULL, &dict_entry))
    {
        std::cerr << "Failed to open dict entry!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    const char *propname_c = prop_name;
    if (!dbus_message_iter_append_basic(&dict_entry, DBUS_TYPE_STRING, &propname_c))
    {
        std::cerr << "Failed to append property name!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    // variant container
    DBusMessageIter variant_iter;
    if (!dbus_message_iter_open_container(&dict_entry, DBUS_TYPE_VARIANT, prop_type_signature, &variant_iter))
    {
        std::cerr << "Failed to open variant container!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    if (strcmp(prop_type_signature, "s") == 0)
    {
        // const char *s = (const char *)prop_value_ptr;
        const char* s = static_cast<const char*>(prop_value_ptr);
        if (!dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_STRING, &s))
        {
            std::cerr << "Failed to append string value!" << std::endl;
            dbus_message_unref(msg);
            return;
        }
    }
    else if (strcmp(prop_type_signature, "i") == 0)
    {
        // int32_t v = *(const int32_t *)prop_value_ptr;
        int32_t v = *static_cast<const int32_t*>(prop_value_ptr);
        if (!dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_INT32, &v))
        {
            std::cerr << "Failed to append int value!" << std::endl;
            dbus_message_unref(msg);
            return;
        }
    }

    dbus_message_iter_close_container(&dict_entry, &variant_iter);
    dbus_message_iter_close_container(&array_iter, &dict_entry);

    dbus_message_iter_close_container(&iter, &array_iter);

    // Append empty as invalidated props (empty array of strings)
    DBusMessageIter inv_iter;
    dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "s", &inv_iter);
    dbus_message_iter_close_container(&iter, &inv_iter);

    // Send signal
    if (!dbus_connection_send(conn, msg, NULL))
    {
        std::cerr << "Failed to send PropertiesChanged signal!" << std::endl;
    }
    else
    {
        std::cout << "Emitted PropertiesChanged signal for " << prop_name << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
}

// ================ Emit Custom Counter Changed Signal ================
// Helper: emit our custom CounterChanged Signal (on our interface)

static void emit_counter_changed(DBusConnection *conn, int32_t newValue)
{
    DBusMessage *msg = dbus_message_new_signal(
        "/xyz/openbmc_project/bhavin/demo",
        "xyz.openbmc_project.Bhavin.Demo.Interface",
        "CounterChanged");

    if (!msg)
        return;

    DBusMessageIter iter;
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT32, &newValue);

    if (!dbus_connection_send(conn, msg, NULL))
    {
        std::cerr << "Failed to send CounterChanged signal!" << std::endl;
    }
    else
    {
        std::cout << "Emitted CounterChanged signal: " << newValue << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
}

// ================ Handler on Object Path ================
// handler for incoming message on our object Path

static DBusHandlerResult message_handler(DBusConnection *conn, DBusMessage *msg, void *user_data)
{

    // identify interface + member
    const char *path = dbus_message_get_path(msg);
    const char *interface = dbus_message_get_interface(msg);
    const char *member = dbus_message_get_member(msg);

    std::cout << "Received message - Path: " << (path ? path : "NULL") << ", Interface: " << (interface ? interface : "NULL") << ", Member: " << (member ? member : "NULL") << std::endl;

    // 1) Introspect

    if (interface && strcmp(interface, "org.freedesktop.DBus.Introspectable") == 0 && member && strcmp(member, "Introspect") == 0)
    {
        DBusMessage *reply = dbus_message_new_method_return(msg);
        if (!reply)
            return DBUS_HANDLER_RESULT_HANDLED;

        const char *xml = introspection_xml;
        dbus_message_append_args(reply, DBUS_TYPE_STRING, &xml, DBUS_TYPE_INVALID);
        dbus_connection_send(conn, reply, NULL);
        dbus_message_unref(reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    // 2) org.freedesktop.DBus.Properties methods: Get, Set, GetAll

    if (interface && strcmp(interface, "org.freedesktop.DBus.Properties") == 0)
    {
        if (member && strcmp(member, "Get") == 0)
        {
            // args: s interface_name, s prop_name
            const char *iface_req = nullptr;
            const char *prop_req = nullptr;
            DBusError err;
            dbus_error_init(&err);

            if (!dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &iface_req, DBUS_TYPE_STRING, &prop_req, DBUS_TYPE_INVALID))
            {
                std::cerr << "Get: bad args: " << err.message << std::endl;
                dbus_error_free(&err);
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            DBusMessage *reply = dbus_message_new_method_return(msg);
            DBusMessageIter iter;
            dbus_message_iter_init_append(reply, &iter);

            // Outer: variant containing the property value
            DBusMessageIter variant_iter;

            if (strcmp(prop_req, "DemoString") == 0)
            {
                const char *out = g_demoString.c_str();
                dbus_message_iter_open_container(&iter, DBUS_TYPE_VARIANT, "s", &variant_iter);
                dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_STRING, &out);
                dbus_message_iter_close_container(&iter, &variant_iter);
            }
            else if (strcmp(prop_req, "DemoCounter") == 0)
            {
                int32_t out = g_demoCounter;
                dbus_message_iter_open_container(&iter, DBUS_TYPE_VARIANT, "i", &variant_iter);
                dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_INT32, &out);
                dbus_message_iter_close_container(&iter, &variant_iter);
            }
            else
            {
                // unknown property: return an empty reply (could return error)
            }

            dbus_connection_send(conn, reply, NULL);
            dbus_message_unref(reply);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if (member && strcmp(member, "GetAll") == 0)
        {
            // arg: s interface_name
            const char *iface_req = nullptr;

            DBusError err;
            dbus_error_init(&err);

            if (!dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &iface_req, DBUS_TYPE_INVALID))
            {
                std::cerr << "GetAll: bad args: " << err.message << std::endl;
                dbus_error_free(&err);
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            std::cout << "GetAll request for interface: " << iface_req << std::endl;

            DBusMessage *reply = dbus_message_new_method_return(msg);
            DBusMessageIter iter;
            dbus_message_iter_init_append(reply, &iter);

            // append a{sv}
            DBusMessageIter array_iter;
            dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &array_iter);

            // DemoString Entry
            DBusMessageIter entry_iter;
            dbus_message_iter_open_container(&array_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
            const char *name1 = "DemoString";
            dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &name1);

            DBusMessageIter var_iter;
            const char *val1 = g_demoString.c_str();
            dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "s", &var_iter);
            dbus_message_iter_append_basic(&var_iter, DBUS_TYPE_STRING, &val1);
            dbus_message_iter_close_container(&entry_iter, &var_iter);
            dbus_message_iter_close_container(&array_iter, &entry_iter);

            // DemoCounter Entry
            dbus_message_iter_open_container(&array_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
            const char *name2 = "DemoCounter";
            dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &name2);
            int32_t val2 = g_demoCounter;
            dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "i", &var_iter);
            dbus_message_iter_append_basic(&var_iter, DBUS_TYPE_INT32, &val2);
            dbus_message_iter_close_container(&entry_iter, &var_iter);
            dbus_message_iter_close_container(&array_iter, &entry_iter);

            dbus_message_iter_close_container(&iter, &array_iter);

            dbus_connection_send(conn, reply, NULL);
            dbus_connection_flush(conn);
            dbus_message_unref(reply);

            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if (member && strcmp(member, "Set") == 0)
        {
            // arg: s interface_name, s prop_name, v value
            // parse the variant and set our property if matches

            std::cout << "Property Set request received!" << std::endl;

            DBusError err;
            dbus_error_init(&err);

            DBusMessageIter args;
            dbus_message_iter_init(msg, &args);

            // interface_name
            if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_STRING)
            {
                std::cerr << "Set: Expected interface name as first argument!" << std::endl;
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            const char *iface_req;
            dbus_message_iter_get_basic(&args, &iface_req);

            dbus_message_iter_next(&args);
            if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_STRING)
            {
                std::cerr << "Set: Expected property name as second argument!" << std::endl;
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            const char *prop_req;
            dbus_message_iter_get_basic(&args, &prop_req);

            std::cout << "Setting property: " << prop_req << " on interface: " << iface_req << std::endl;

            dbus_message_iter_next(&args);
            if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_VARIANT)
            {
                std::cerr << "Set: Expected variant as third argument" << std::endl;
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            DBusMessageIter variant_iter;
            dbus_message_iter_recurse(&args, &variant_iter);
            int inner_type = dbus_message_iter_get_arg_type(&variant_iter);

            if (strcmp(prop_req, "DemoString") == 0 && inner_type == DBUS_TYPE_STRING)
            {
                const char *newval;
                dbus_message_iter_get_basic(&variant_iter, &newval);

                std::cout << "Setting DemoString to: " << newval << std::endl;
                g_demoString = std::string(newval);

                // reply with empty method return
                DBusMessage *reply = dbus_message_new_method_return(msg);
                dbus_connection_send(conn, reply, NULL);
                dbus_connection_flush(conn);
                dbus_message_unref(reply);

                // emit PropertiesChanged and also our custom signal
                const char *iface_c = "xyz.openbmc_project.Bhavin.Demo.Interface";
                const char *str_val = g_demoString.c_str();
                emit_properties_changed(conn, iface_c, "DemoString", "s", str_val);
                return DBUS_HANDLER_RESULT_HANDLED;
            }
            else if (strcmp(prop_req, "DemoCounter") == 0 && inner_type == DBUS_TYPE_INT32)
            {
                int32_t newval;
                dbus_message_iter_get_basic(&variant_iter, &newval);

                std::cout << "Setting DemoCounter to: " << newval << std::endl;
                g_demoCounter = newval;

                DBusMessage *reply = dbus_message_new_method_return(msg);
                dbus_connection_send(conn, reply, NULL);
                dbus_connection_flush(conn);
                dbus_message_unref(reply);

                const char *iface_c = "xyz.openbmc_project.Bhavin.Demo.Interface";
                emit_properties_changed(conn, iface_c, "DemoCounter", "i", &g_demoCounter);
                emit_counter_changed(conn, g_demoCounter);
                return DBUS_HANDLER_RESULT_HANDLED;
            }
            else
            {
                std::cerr << "Set: Unsupported property or type mismatch!" << std::endl;
            }

            return DBUS_HANDLER_RESULT_HANDLED;
        }
    } // end Properties Handling

    // 3) Our Custom interface methods: IncrementCounter, ResetCounter
    if (interface && strcmp(interface, "xyz.openbmc_project.Bhavin.Demo.Interface") == 0)
    {
        if (member && strcmp(member, "IncrementCounter") == 0)
        {
            std::cout << "IncrementCounter called!" << std::endl;
            g_demoCounter += 1;

            // reply empty
            DBusMessage *reply = dbus_message_new_method_return(msg);
            dbus_connection_send(conn, reply, NULL);
            dbus_connection_flush(conn);
            dbus_message_unref(reply);

            // emit standard PropertiesChanged for DemoCounter
            const char *iface_c = "xyz.openbmc_project.Bhavin.Demo.Interface";
            emit_properties_changed(conn, iface_c, "DemoCounter", "i", &g_demoCounter);
            emit_counter_changed(conn, g_demoCounter);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if (member && strcmp(member, "ResetCounter") == 0)
        {
            std::cout << "ResetCounter called!" << std::endl;
            g_demoCounter = 0;

            DBusMessage *reply = dbus_message_new_method_return(msg);
            dbus_connection_send(conn, reply, NULL);
            dbus_connection_flush(conn);
            dbus_message_unref(reply);

            const char *iface_c = "xyz.openbmc_project.Bhavin.Demo.Interface";
            emit_properties_changed(conn, iface_c, "DemoCounter", "i", &g_demoCounter);
            emit_counter_changed(conn, g_demoCounter);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
    }

    // Not handled: return not-yet-handled
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

// ================ Obaject vtable ================

static DBusObjectPathVTable obj_vtable = {
    NULL,
    message_handler,
    NULL,
    NULL,
    NULL,
    NULL};

// signal handler for clean shutdown
static volatile bool running = true;
void signal_handler(int sig)
{
    std::cout << "\nReceived signal " << sig << " , shutting down..." << std::endl;
    running = false;
}

// ================ Main Function ================

int main()
{

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    DBusError err;
    dbus_error_init(&err);

    // Connect to system Bus (match how other system services run in OpenBMC/QEMU)
    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (!conn)
    {
        std::cerr << "Failed to connect to System Bus!!!\n"
                  << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    // request a well known name on the bus

    int request = dbus_bus_request_name(conn, "xyz.openbmc_project.Bhavin.Demo", DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);

    if (dbus_error_is_set(&err))
    {
        std::cerr << "RequestName error: " << err.message << std::endl;
        dbus_error_free(&err);
    }

    if (request != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
        std::cerr << "Warning: Did not become primary owner ( request = " << request << " )" << std::endl;
    }

    // Register Object
    if (!dbus_connection_register_object_path(conn, "/xyz/openbmc_project/bhavin/demo", &obj_vtable, NULL))
    {
        std::cerr << "Failed to register object path!" << std::endl;
        return 1;
    }

    std::cout << "Service started!\n - Name: xyz.openbmc_project.Bhavin.Demo\n - Path: /xyz/openbmc_project/bhavin/demo\n"
              << std::endl;

    // Main loop : read/write/dispatch (blocking)
    while (running)
    {
        // -1 block until message arrives
        if (!dbus_connection_read_write_dispatch(conn, 1000))
        {
            if(!dbus_connection_get_is_connected(conn)){
                std::cerr<<"DBus disconnected!"<<std::endl;
                running = false;
            }
            // std::cerr << "Connection lost!" << std::endl;
            // break;
        }
    }

    std::cout << "Service shutting down...!" << std::endl;
    dbus_connection_flush(conn);

    return 0;
}
