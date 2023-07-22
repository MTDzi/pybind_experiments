from torch.utils.cpp_extension import load


torch_functions_cpp = load(name="torch_functions_cpp", sources=["torch_functions.cpp"], verbose=True)
help(torch_functions_cpp)
