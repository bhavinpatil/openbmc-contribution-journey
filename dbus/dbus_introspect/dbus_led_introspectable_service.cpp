#include <dbus-1.0/dbus/dbus.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

const char *OBJECT_PATH = "/com/example/LEDService";
const char *INTERFACE_NAME = "com.example.Led.Control";
const char *SERVICE_NAME = "com.example.LEDService";

// Static XML string used for intropection only
const char *intropect_xml = "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
                            " \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
                            "<node>\n"
                            "  <interface name=\"org.freedesktop.DBus.Introspectable\">\n"
                            "    <method name=\"Introspect\">\n"
                            "      <arg name=\"data\" type=\"s\" direction=\"out\"/>\n"
                            "    </method>\n"
                            "  </interface>\n"
                            "  <interface name=\"xyz.openbmc_project.Led.Control\">\n"
                            "    <property name=\"LEDState\" type=\"s\" access=\"readwrite\"/>\n"
                            "    <signal name=\"LEDStateChanged\">\n"
                            "      <arg type=\"s\" name=\"new_state\"/>\n"
                            "    </signal>\n"
                            "  </interface>\n"
                            "</node>";

int main()
{
    DBusError err;
    DBusConnection *conn;

    dbus_error_init(&err);

    // Step 1: connect to session bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if (dbus_error_is_set(&err))
    {
        std::cerr << "Connection Error: " << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    // Step 2: request service name
    int ret = dbus_bus_request_name(conn, SERVICE_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err))
    {
        std::cerr << "Connection Error: " << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    std::cout << "IntrospectService:: Running at " << OBJECT_PATH << std::endl;

    while (true)
    {
        dbus_connection_read_write(conn, 0);

        DBusMessage *msg = dbus_connection_pop_message(conn);
        if (!msg)
        {
            usleep(100000);
            continue;
        }

        // Step 3: Handle Introspect Call
        if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect") &&
            strcmp(dbus_message_get_path(msg), OBJECT_PATH) == 0)
        {
            std::cout << "IntrospectService:: Received Introspect call" << std::endl;

            // Step 4: create reply message with XML
            DBusMessage *reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);

            const char *xml = intropect_xml;

            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &xml);

            // Step 5 : send reply
            dbus_connection_send(conn, reply, NULL);
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }

    return 0;
}