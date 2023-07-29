# Introduction

This is a Capstone Project for the Udacity's C++ Nano-Degree.

The purpose of this project was to explore `pybind11` and to:
1. Gather a collection of working examples
2. Craft a working development environment (with a proper debugging tools)
3. Play a bit with pybinding PyTorch's C++ API
such that I could apply this knowledge to my everyday work which is mainly Python-related.


## Motivation

My biggest difficulty with C++ was always not the language itself (I mean... the syntax + its various idiosyncrasies are not the greatest, but with a right debugging setup they're managable) but environment setup (compiling / linking, debugging, but also: how to debug build systems such as CMake), thus I figured I'll develop a Capstone Project that combines two most important thigs from my perspective: how to enhance my Python skills with C++, and get more familiar with the env setup itself.


## Build

I built this project with:
* Python 3.8
* g++ 7.5
but for Python we'll need a virtualenv.


### Python virtualenv

Let's first create and activate a virtualenv called `pybind_env` (I'm using `python3.8` but any version >= 3.8 should be OK):
```bash
python3.8 -m venv pybind_env
source pybind_env/bin/activate
```
We'll also need the latest version of `pip` for installing packages:
```bash
pip install --upgrade pip
```
and now we can install the dependencies:
```bash
pip install -r requirements.txt
```
Now we're all set to build the project.

### Building the project

The project is composed of two Python packages (each with its own `setup.py` file).

To (re)build the project in debug mode, run:
```bash
DEBUG=1 TORCH_CMAKE_PREFIX_PATH=`python -c 'import torch.utils; print(torch.utils.cmake_prefix_path)'` pip install --no-clean --upgrade -e .
```

and to (re)build it in release mode, run:
```bash
TORCH_CMAKE_PREFIX_PATH=`python -c 'import torch.utils; print(torch.utils.cmake_prefix_path)'` pip install --upgrade -e .
```

There's also a sub-package, `torch_utils`, which needs to built with:
```bash
pip install --upgrade -e torch_utils
```
and
```bash
DEBUG=1 pip install --no-clean --upgrade -e torch_utils
```
for debugging.


## Running the project

All functions and classes are utilized in the `main.py` script and can be run by calling:
```bash
python main.py
```
and the expected output should read:
```
timeit(py_subtract_n_times, args_set_0) = 30.773[ms] (x6602.1 wrt the fastest)
timeit(numba_subtract_n_times, args_set_0) = 0.005[ms] (x1.0 wrt the fastest)
timeit(cpp_subtract_n_times, args_set_0) = 0.005[ms] (x1.0 wrt the fastest)

timeit(py_<lambda>, args_set_0) = 13.650[ms] (x4.4 wrt the fastest)
timeit(cpp_<lambda>, args_set_0) = 3.111[ms] (x1.0 wrt the fastest)

timeit(py_subtract_n_times, args_set_0) = 584.810[ms] (x71240.1 wrt the fastest)
timeit(cpp_subtract_tensor_n_times, args_set_0) = 0.008[ms] (x1.0 wrt the fastest)
```

### Debugging

I wanted to be able to debug the Python script as well as the C++ library's code that's being called from within the Python script.

I followed [this](https://stackoverflow.com/questions/71125094/debug-a-python-c-c-pybind11-extension-in-vscode-linux) thread on Stack Overflow to setup a proper development environment.

I disabled `ptrace_scope` so that we won't be asked to authenticate when attaching to a process (taken from [here](https://github.com/Microsoft/MIEngine/wiki/Troubleshoot-attaching-to-processes-using-GDB)):
```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```

I recorded my screen to give a sense of what I was trying to achieve:
[![Watch the video](https://img.youtube.com/vi/4nGcLkVcF9o/hqdefault.jpg)](https://www.youtube.com/embed/4nGcLkVcF9o)


# Functions and classes

My purpose was to create a framework for further development (mainly for my day job) and not developing a super-duper project utilizing `pybind11`. Thus, the functions and classes in this project are not very sophisticated, but they served their purpose.

The `simple_functions.cpp` file holds one function, `subtract_n_times` whose Python counterpart is in the `simple_functions.py` module.

A bit more complicated is the `astar.cpp` file containing three classes: `Node`, `Grid`, and `AStar`. The `Grid` and `AStar` classes are exposed for Python, and implement the cost map, and the A-star algorithm. `AStar` uses `std::async` to evaluate neighbors in parallel.

But there's one significant aspect of my everyday work that I wanted to pursue as well, and that is: how to `pybind11` with `torch`? And this part took most of the time I dedicated to the project, to be honest.

# Binding with `torch`

## `libtorch` (which didn't work)
Initially, I was trying to work with `libtorch` (can be downloaded [here](https://pytorch.org/get-started/locally/)), and it worked in the sense that the main package was successfully built. However, when I tried importing the module built with `libtorch` I kept getting the following error:
```python
>>> import torch_functions as cpp_torch_func
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: /home/md/Desktop/pybind_experiments/torch_functions.cpython-38-x86_64-linux-gnu.so: undefined symbol: _ZN8pybind116detail11type_casterIN2at6TensorEvE4loadENS_6handleEb
```
and it seems this is due to the fact that the version of `pybind11` needs to match the version used when building `torch`. But even when I made sure the versions match, I still kept getting this error.

As a result of digging deeper into it, I stumbled upon the examples provided by PyTorch developers. One of them, detailed below, worked!


## Natively installed `torch` approach (which worked)
Following the example in the [`extension-cpp` repo](https://github.com/pytorch/extension-cpp/blob/master/cpp/setup.py), allowed me to have a working sub-package (with its own, dedicated `setup.py` file).

Under the hood, that sub-package uses:
```python
from torch.utils.cpp_extension import BuildExtension, CppExtension
```
which takes care of using the correct version of `pybind11`, matching compile flags and options.

This is a much cleaner approach, because with out it, a newer version of `torch` would necessitate adapting the `CMakeLists.txt` every time.
