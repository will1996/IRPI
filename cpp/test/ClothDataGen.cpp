#include"ClothSimulator.hpp"
#include"assert.h"
using namespace IRPI::Sim;

int main(){
    ClothSimulator sim;
    assert(sim.Simulate()=="SIMULATING");
    assert(sim.Configure()=="CONFIGURING");
}