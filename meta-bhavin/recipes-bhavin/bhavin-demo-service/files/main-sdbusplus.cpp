#include <boost/asio/io_context.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <iostream>
#include <memory>

class DemoService
{
  public:
    DemoService(boost::asio::io_context& io) :
        io(io),
        conn(std::make_shared<sdbusplus::asio::connection>(io)),
        server(std::make_shared<sdbusplus::asio::object_server>(conn))
    {
        // Request a well-known name
        conn->request_name("xyz.openbmc_project.Bhavin.Demo");

        // Add interface at object path
        iface = server->add_interface(
            "/xyz/openbmc_project/bhavin/demo",
            "xyz.openbmc_project.Bhavin.Demo.Interface");

        // Register writable properties
        iface->register_property("DemoCounter", demoCounter);
        iface->register_property("DemoString", demoString);

        // Register methods
        iface->register_method("IncrementCounter", [this]() {
            demoCounter++;
            std::cout << "IncrementCounter called, value=" << demoCounter
                      << std::endl;
            return demoCounter;
        });

        iface->register_method("ResetCounter", [this]() {
            demoCounter = 0;
            std::cout << "ResetCounter called, value=" << demoCounter
                      << std::endl;
            return demoCounter;
        });

        iface->initialize();

        std::cout << "Service started!" << std::endl
                  << " - Name: xyz.openbmc_project.Bhavin.Demo" << std::endl
                  << " - Path: /xyz/openbmc_project/bhavin/demo" << std::endl;
    }

    void run()
    {
        io.run();
    }

  private:
    boost::asio::io_context& io;
    std::shared_ptr<sdbusplus::asio::connection> conn;
    std::shared_ptr<sdbusplus::asio::object_server> server;
    std::shared_ptr<sdbusplus::asio::dbus_interface> iface;

    int demoCounter = 0;
    std::string demoString = "HelloWorld";
};

int main()
{
    boost::asio::io_context io;
    DemoService service(io);
    service.run();
    return 0;
}
