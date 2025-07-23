#include <unistd.h> //for pipes, fork, read, write
#include <dbus-1.0/dbus/dbus.h>
#include <iostream>
#include <sys/wait.h>
#include <cstring>
#include <sys/types.h>
#include <cstdlib>

// function to emit the DBus signal from parent

void send_dbus_signal()
{
    DBusError err;
    DBusConnection *conn;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    // register the sender name
    dbus_bus_request_name(conn, "com.example.SignalPipe", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    DBusMessage *signal;
    DBusMessageIter args;

    const char *info = "read_ready";

    // Create the DBus Signal
    signal = dbus_message_new_signal(
        "/com/example/SignalPipe",         // object path
        "com.example.SignalPipeInterface", // interface
        "NotifyChild"                      // signal name
    );

    dbus_message_iter_init_append(signal, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &info);

    dbus_connection_send(conn, signal, NULL);
    dbus_connection_flush(conn);

    std::cout << "parent::send D-Bus signal - NotifyChild(\"" << info << "\")\n";

    dbus_message_unref(signal);
}

// function to act as child process: listen for signal and reads from pipe

void child_process(int pipe_read_fd)
{
    DBusError err;
    DBusConnection *conn;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    // subscribe to the signal NotifyChild

    dbus_bus_add_match(conn, "type='signal', interface='com.example.SignalPipeInterface', member='NotifyChild'", &err);

    dbus_connection_flush(conn);

    std::cout << "child::waiting for dbus signal...." << std::endl;

    while (true)
    {
        dbus_connection_read_write(conn, 0);

        DBusMessage *msg = dbus_connection_pop_message(conn);

        if (!msg)
        {
            usleep(100000);
            continue;
        }

        if (dbus_message_is_signal(msg, "com.example.SignalPipeInterface", "NotifyChild"))
        {
            std::cout << "child::signal received! now reading from pipe...." << std::endl;

            // read from the pipe
            char buffer[128] = {0};
            ssize_t bytes = read(pipe_read_fd, buffer, sizeof(buffer));
            if (bytes > 0)
            {
                std::cout << "child::pipe message - " << buffer << std::endl;
            }
            else
            {
                std::cout << "child::failed to read from pipe!" << std::endl;
            }

            dbus_message_unref(msg);
            break; // exit loop after one signal
        }

        dbus_message_unref(msg);
    }
}

int main()
{
    int pipefd[2]; // pipefd[0] - read end :: pipefd[1] - write end

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        // child process : close write end and wait for signal
        close(pipefd[1]);
        child_process(pipefd[0]);
        close(pipefd[0]);
        return 0;
    }
    else
    {
        // parent process : close read end and send signal
        close(pipefd[0]);
        // const char* parent_message = "Data is ready via pipe!";
        std::string status[] = {"Preparing data", "Data is ready", "Sending Data"};
        for (const auto &st : status)
        {
            std::cout << "parent::writing message into the pipe - "<< st << std::endl;
            write(pipefd[1], st.c_str(), st.length());
            sleep(1); // simulate delay before siginaling
        }
        // write(pipefd[1], parent_message, strlen(parent_message));
        // sleep(1); //simulate delay before siginaling

        std::cout << "parent::sending dbus signal....." << std::endl;
        send_dbus_signal();
        close(pipefd[1]);
        // waitpid(pid, nullptr, 0); //wait for child to finish
        wait(NULL);

        std::cout << "parent::child has exited!" << std::endl;
    }

    std::cout << "parent::exiting..." << std::endl;

    return 0;
}
