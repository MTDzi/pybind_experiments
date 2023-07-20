I followed [this](https://stackoverflow.com/questions/71125094/debug-a-python-c-c-pybind11-extension-in-vscode-linux) thread on Stack Overflow to setup a proper development environment.


Disable `ptrace_scope` so that we won't be asked to authenticate when attaching to a process (taken from [here](https://github.com/Microsoft/MIEngine/wiki/Troubleshoot-attaching-to-processes-using-GDB))
```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```

To (re)build the project in debug mode, run:
```bash
DEBUG=1 pip install --no-clean --upgrade -e .
```

and to (re)build it in release mode, I run:
```bash
pip install --upgrade -e .
```

## torch
python -m pip install torch==2.0.1+cpu --index-url https://download.pytorch.org/whl/cpu

## libtorch
Get it from: https://pytorch.org/get-started/locally/
```bash
wget https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-latest.zip 
```
and
```bash
unzip libtorch-shared-with-deps-latest.zip
```