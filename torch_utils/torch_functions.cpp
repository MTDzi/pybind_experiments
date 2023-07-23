#include <torch/extension.h>


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def(
    "subtract_tensor_n_times",
    [](torch::Tensor i, torch::Tensor j, int n) {
        auto result = i;
        for (int index=0; index++; index<n)
          result -= j;
        return result;
    }, 
    R"pbdoc(
        Subtract the second tensor from the first one n times, in a loop.
    )pbdoc", 
    py::arg("i"),
    py::arg("j"),
    py::arg("n") = 10000
  );
}
