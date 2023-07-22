#include <torch/extension.h>


torch::Tensor subtract_tensors_n_times2(torch::Tensor i, torch::Tensor j, int n) {
  return i + j;
}


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
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

  m.def("subtract_tensors_n_times2", &subtract_tensors_n_times2);
}
