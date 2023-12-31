from typing import Union

from numba import njit
import torch


def subtract_n_times(i: Union[int, torch.Tensor], j: Union[int, torch.Tensor], times: int) -> int:
    result = i
    for _ in range(times):
        result -= j
    return result


numba_subtract_n_times = njit('int64(int64, int64, int64)')(subtract_n_times)

