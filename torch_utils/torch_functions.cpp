#include <torch/extension.h>


torch::Tensor subtract_tensor_n_times(torch::Tensor i, torch::Tensor j, int n) {
  auto result = i;
  for (int index=0; index++; index<n) {
    result -= j;
  }
  return result;
}


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("subtract_tensor_n_times", &subtract_tensor_n_times);
}
