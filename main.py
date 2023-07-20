import timeit

import numpy as np
import torch

import src.simple_functions as py_func
import src.astar as py_astar
import simple_functions as cpp_func
import astar as cpp_astar
import torch_functions as cpp_torch_functions
cpp_torch_functions.subtract_tensors_n_times(torch.Tensor([0, 1, 2], device='cpu'), torch.Tensor([3,4,5], device='cpu'), 100)



def produce_path(GridClass, AStarClass, grid_size=20):
    # Create a grid
    g = GridClass(grid_size, grid_size)
    
    # Add a small wall
    g.setObstacle(1, 0)
    g.setObstacle(1, 1)
    g.setObstacle(1, 2)
    g.setObstacle(1, 3)
    g.setObstacle(1, 4)
    
    # Always go from upper left corner to bottom right
    astar = AStarClass(g, 0, 0, grid_size - 1, grid_size - 1)
    astar.findPath()
    path = astar.getPath()
    
    # Make sure that the path is of the expected length
    assert len(path) == (2 * grid_size - 1)


def main():
    use_cases = [
        {
            'functions': {
                'py': py_func.subtract_n_times,
                'numba': py_func.numba_subtract_n_times,
                'cpp': cpp_func.subtract_n_times,
            },
            'args_sets': [(1, 2, 100000)]
        },
        {
            'functions': {
                'py': lambda: produce_path(cpp_astar.Grid, cpp_astar.AStar),
                'cpp': lambda: produce_path(py_astar.Grid, py_astar.AStar),
            },
            'args_sets': [()]
        }
    ]
    
    num_repetitions = 1000
    for use_case in use_cases:
        functions = use_case['functions']
        args_sets = use_case['args_sets']
        execution_times = {}
        for args_set_idx, args_set in enumerate(args_sets):
            execution_times[args_set_idx] = {}
            for prefix, function in functions.items():
                execution_time = timeit.timeit(lambda: function(*args_set), number=num_repetitions)
                execution_times[args_set_idx][prefix] = execution_time
                
        for args_set_idx, args_set in enumerate(args_sets):
            fastest = min(execution_times[args_set_idx].values())
            for prefix, function in functions.items():
                execution_time = execution_times[args_set_idx][prefix]
                print(f'timeit({prefix}_{function.__name__}, args_set_{args_set_idx}) = {1000 * execution_time:.3f}[ms] (x{(execution_time / fastest):.1f} wrt the fastest)')


if __name__ == '__main__':
    main()