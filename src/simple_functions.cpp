#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

PYBIND11_MODULE(simple_functions, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: simple_functions

        .. autosummary::
           :toctree: _generate

           subtract_n_times
    )pbdoc";

    m.def(
        "subtract_n_times",
        [](int i, int j, int n) {
            int result = i;
            for (int index=0; index++; index<n)
                result -= j;
            return result;
        }, 
        R"pbdoc(
            Subtract the second number from the first one n times.
        )pbdoc", 
        py::arg("i"),
        py::arg("j"),
        py::arg("n") = 10000
    );



#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
