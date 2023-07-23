import os

from setuptools import setup
from torch.utils.cpp_extension import BuildExtension, CppExtension


DEBUG_FLAGS = ['-g'] if os.getenv('DEBUG') else []


setup(
    name='torch_utils',
    ext_modules=[
        CppExtension(
            'cpp',
            ['torch_functions.cpp'],
            extra_compile_args=DEBUG_FLAGS,
        ),
    ],
    cmdclass={
        'build_ext': BuildExtension.with_options(
            use_ninja=False,
        )
    })
