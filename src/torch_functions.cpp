#include <torch/torch.h>
#include <torch/extension.h>
#include <ATen/ATen.h>


#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

PYBIND11_MODULE(torch_functions, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: torch_functions

        .. autosummary::
           :toctree: _generate

           subtract_tensors_n_times
    )pbdoc";

    m.def(
        "subtract_tensors_n_times",
        [](torch::Tensor i, torch::Tensor j, int n) {
            return i;
        }, 
        R"pbdoc(
            TODO.
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
