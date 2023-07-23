import os

from setuptools import setup
from torch.utils.cpp_extension import BuildExtension, CppExtension


DEBUG_FLAGS = ['-g', '-O0'] if os.getenv('DEBUG') else []

print(f'DEBUG_FILES = {DEBUG_FLAGS}')

setup(
    name='torch_utils',
    version="0.0.1",
    ext_modules=[
        CppExtension(
            'cpp',
            ['torch_functions.cpp'],
            extra_compile_args=DEBUG_FLAGS,
        ),
    ],
    cmdclass={
        'build_ext': BuildExtension.with_options(
            use_ninja=True,
            extra_compile_args=DEBUG_FLAGS,
        )
    })
