from numba import njit


def subtract_n_times(i: int, j: int, times: int) -> int:
    result = i
    for _ in range(times):
        result -= j
    return result


numba_subtract_n_times = njit('int64(int64, int64, int64)')(subtract_n_times)

