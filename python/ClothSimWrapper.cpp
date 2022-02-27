#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include"ClothSimulator.hpp"
namespace py = pybind11;

void init_clothSim(py::module &m) {
    py::class_<IRPI::Sim::ClothSimulator>(m, "ClothSimulator")
    .def(py::init<>())
    .def("Configure",
         py::overload_cast<>( &IRPI::Sim::ClothSimulator::Configure))
    .def("Simulate",
         py::overload_cast<>( &IRPI::Sim::ClothSimulator::Simulate));
}