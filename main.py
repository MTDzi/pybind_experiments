import timeit

import src.simple_functions as py
import pybind_experiments as cpp



def main():
    assert cpp.add(1, 2) == 3
    assert cpp.subtract(1, 2) == -1
            
    use_cases = [
        {
            'functions': {
                'py': py.subtract_n_times,
                'numba': py.numba_subtract_n_times,
                'cpp': cpp.subtract_n_times,
            },
            'args_sets': [(1, 2, 100000)]
        }
    ]
    
    num_repetitions = 1000
    for use_case in use_cases:
        functions = use_case['functions']
        args_sets = use_case['args_sets']
        for prefix, function in functions.items():
            for args_set_idx, args_set in enumerate(args_sets):
                execution_time = timeit.timeit(lambda: function(*args_set), number=num_repetitions)
                print(f'timeit({prefix}_{function.__name__}, args_set_{args_set_idx}) = {1000 * execution_time:.3f}[ms]')

if __name__ == '__main__':
    main()