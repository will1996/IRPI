
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_clothSim(py::module &);

namespace mcl {

PYBIND11_MODULE(IRPIPy, m) {
    // Optional docstring
    m.doc() = "InverseRendering as Physiscs Ininitialization";
    init_clothSim(m);
}
}