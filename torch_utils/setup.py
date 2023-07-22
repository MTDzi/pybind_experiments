from setuptools import setup
from torch.utils.cpp_extension import BuildExtension, CppExtension

setup(
    name='torch_utils',
    ext_modules=[
        CppExtension(
            'cpp',
            ['torch_functions.cpp'],
            extra_compile_args=['-g'],
        ),
    ],
    cmdclass={
        'build_ext': BuildExtension.with_options(
            use_ninja=False,
        )
    })
