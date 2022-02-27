#pragma once
#include<string>
namespace IRPI::Sim{
    class ClothSimulator{
        public:
        ClothSimulator() = default;

        std::string Configure(){
                return "CONFIGURING";
        }




        std::string Simulate(){
                return "SIMULATING";
        }
    };
}