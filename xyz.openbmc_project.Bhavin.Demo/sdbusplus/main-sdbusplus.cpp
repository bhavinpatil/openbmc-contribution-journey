//main.cpp - using sdbusplus C++ binding

//bhavin-demo-service D-Bus service rewritten using sdbusplus (instead of libdbus-1)
// Service: xyz.openbmc_project.Bhavin.Demo
// Path: /xyz/openbmc_project/bhavin/demo
// Interface: xyz.openbmc_project.Bhavin.Demo.Interface


#include<sdbusplus/bus.hpp>
#include<sdbusplus/server/interface.hpp>
#include<sdbusplus/server/object.hpp>
#include<iostream>
#include<string>
#include<csignal>
#include<atomic>

//Global running flag for signal handling

static std::atomic<bool> running(true);

void signal_handler(int){
    running = false;
}


//Our service implementation class
class DemoService
{
    public:
        DemoService(sdbusplus::bus_t& bus, const char* path):bus(bus), iface(bus, path, "xyz.openbmc_project.Bhavin.Demo.Interface", true){
            //Initialize properties
            demoString = "HelloWorld";
            demoCounter = 0;

            //Register methods
            iface.register_method("IncrementCounter", [this](){
                std::cout<<"IncrementCounter called!"<<std::endl;
                demoCounter++;
                iface.property_changed("DemoCounter");
                emitCounterChanged(demoCounter);
            });

            iface.register_method("ResetCounter", [this](){
                std::cout<<"ResetCounter called!"<<std::endl;
                demoCounter = 0;
                iface.property_changed("DemoCounter");
                emitCounterChanged(demoCounter);
            })

            //Register properties
            iface.register_property("DemoString", demoString, sdbusplus::vtable::property_::emits_change, [this](const std::string& value){
                std::cout<<"Setting DemoString to: "<<value<<std::endl;
                demoString = value;
                return demoString;
            },
            [this](){return demoString; });

            iface.register_property("DemoCounter", demoCounter, sdbusplus::vtable::property_::emit_change, [this](const __int32_t& value){
                std::cout<<"Setting DemoCounter to: "<<value<<std::endl;
                demoCounter = value;
                emitCounterChanged(demoCounter);
                return demoCounter;
            },
            [this](){return demoCounter; });


            //Finish registration
            iface.initialize();
        }
    
        void emitCounterChanged(__int32_t newValue){
            auto sig = iface.new_signal("CounterChanged");
            sig.append(newValue);
            sig.signal_send();
            std::cout<<"Emitted CounterChanged signal: "<<newValue<<std::endl;
        }

    private:
        
        sdbusplus::bus_t& bus;
        sdbusplus::server::interface_t iface;

        std::string demoString;
        __int32_t demoCounter;
};


int main()
{
    //handle SIGINT/SIGTERM
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);


    //connect to system bus
    auto bus = sdbusplus::bus::new_system();


    //request well-known name
    bus.request_name("xyz.openbmc_project.Bhavin.Demo");

    //Create our service
    DemoService demo(bus, "/xyz/openbmc_project/bhavin/demo");

    std::cout<<"Service started!\n"<<" - Name: xyz.openbmc_project.Bhavin.Demo\n"<<" - Path: /xyz/openbmc_project/bhavin/demo\n"<<std::endl;

    //main while loop
    while(running){
        bus.process_discard();
        bus.wait();
    }


    std::cout<<"Service shutting down...."<<std::endl;
    return 0;
}