import time
import numpy as np
from multiprocessing import shared_memory
import posix_ipc  # pip install posix_ipc for semaphores
import pyvista as pv  # For 3D rendering
import sys


SHM_NAME = "/voxelGrid"
SEM_NAME = "/voxelGridSemaphore"


if __name__ == "__main__":
    gridDim = int(sys.argv[1])

    bufferSize = gridDim * gridDim * gridDim

    shm = shared_memory.SharedMemory(name=SHM_NAME, create=False, size=bufferSize)

    array_view = np.ndarray((gridDim, gridDim, gridDim), dtype=np.uint8, buffer=shm.buf)

    update_sem = posix_ipc.Semaphore(SEM_NAME, flags=posix_ipc.O_CREAT, initial_value=0)

    plotter = pv.Plotter(off_screen=False)  
    volume = pv.UniformGrid(dimensions=(gridDim, gridDim, gridDim))
    volume['data'] = array_view.ravel()

    plotter.add_volume(volume, scalars="values", opacity="sigmoid", cmap="viridis")
    plotter.show(auto_close=False)

    while True:
        update_sem.acquire()

        volume['data'] = array_view.ravel()
        plotter.update()

        update_sem.release()

        time.sleep(0.01)  # Throttle if needed... will need to test

    # Cleanup
    shm.close()
    update_sem.close()
    posix_ipc.unlink_semaphore(SEM_NAME)