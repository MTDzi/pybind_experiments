# Introduction

This is a Capstone Project for the Udacity's C++ Nano-Degree.

The purpose of this project was to explore `pybind11` and to:
1. Gather a collection of working examples
2. Craft a working development environment (with a proper debugging tools)
3. Play a bit with pybinding PyTorch's C++ API
such that I could apply this knowledge to my everyday work which is mainly Python-related.


## Motivation

My biggest difficulty with C++ was always not the language itself (I mean... the syntax + its various idiosyncrasies are not the greatest, but ) but environment setup (compiling / linking, debugging, but also: how to debug build systems such as CMake), thus I figured I'll develop a Capstone Project that combines two most important thigs from my perspective: how to enhance my Python skills with C++, and get more familiar with the env setup itself.


## Building the project

The project is build using two 

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
DEBUG=1 pip install --upgrade -e torch_utils
```
for debugging.

## Debugging

I wanted to be able to debug the Python script as well as the C++ library's code that's being called from within the Python script.

I followed [this](https://stackoverflow.com/questions/71125094/debug-a-python-c-c-pybind11-extension-in-vscode-linux) thread on Stack Overflow to setup a proper development environment.

I disabled `ptrace_scope` so that we won't be asked to authenticate when attaching to a process (taken from [here](https://github.com/Microsoft/MIEngine/wiki/Troubleshoot-attaching-to-processes-using-GDB)):
```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```

I recorded my screen to give a sense of what I was trying to achieve:
[![Watch the video](https://img.youtube.com/vi/4nGcLkVcF9o/hqdefault.jpg)](https://www.youtube.com/embed/4nGcLkVcF9o)


# What worked

## `torch`
Following the example in the [`extension-cpp` repo](https://github.com/pytorch/extension-cpp/blob/master/cpp/setup.py), 
```bash
python -m pip install torch==2.0.1+cpu --index-url https://download.pytorch.org/whl/cpu
```

# What didn't work

## `libtorch`
Initially, I was trying to work with `libtorch` (can be downloaded [here](https://pytorch.org/get-started/locally/)).
