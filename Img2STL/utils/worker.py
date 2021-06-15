import wx

from multiprocessing import Process, Queue
from ..stl import Vertex3, Polygon3, STLFile
from .utils import WorkerMessage, WMCmds


class Worker(Process):
    def __init__(self, tid: int, den: float, q_from_exec: Queue, q_to_exec: Queue) -> None:
        super().__init__(name=f"Worker-{tid}", daemon=True)
        self._density = den
        self._q_from_exec = q_from_exec
        self._q_to_exec = q_to_exec
        self._file = STLFile()

        wx.LogMessage(f"{self.name} created.")

    def run(self) -> None:

        while True:
            msg: WorkerMessage = self._q_from_exec.get()

            if msg.cmd == WMCmds.wMSG_RUN:
                self._pixel_calc(msg)
            elif msg.cmd == WMCmds.wMSG_END:
                self._q_to_exec.put(self._file)
                break
            elif msg.cmd == WMCmds.wMSG_STOP:
                break

    def _pixel_calc(self, msg: WorkerMessage) -> None:
        if msg.z0 != 0:
            vx_1 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_2 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_3 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            vx_4 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            nrm = Vertex3(0.0, 0.0, 1.0)

            self._file.add_triangle(Polygon3(nrm, vx_3, vx_2, vx_4))
            self._file.add_triangle(Polygon3(nrm, vx_1, vx_2, vx_3))

            vx_1 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           0.0)
            vx_2 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           0.0)
            vx_3 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           0.0)
            vx_4 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           0.0)
            nrm = Vertex3(0.0, 0.0, -1.0)

            self._file.add_triangle(Polygon3(nrm, vx_1, vx_3, vx_2))
            self._file.add_triangle(Polygon3(nrm, vx_3, vx_4, vx_2))

        if msg.z1 != msg.z0:
            vx_1 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_2 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           msg.z1)
            vx_3 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            vx_4 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           msg.z1)
            nrm = Vertex3(1.0, 0.0, 0.0)
            if msg.z1 >= msg.z0:
                nrm.x = -1.0

            self._file.add_triangle(Polygon3(nrm, vx_1, vx_2, vx_3))
            self._file.add_triangle(Polygon3(nrm, vx_2, vx_4, vx_3))

        if (msg.x == 0) and (msg.z0 > 0):
            vx_1 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_2 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            vx_3 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           0.0)
            vx_4 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           0.0)
            nrm = Vertex3(-1.0, 0.0, 0.0)

            self._file.add_triangle(Polygon3(nrm, vx_1, vx_2, vx_3))
            self._file.add_triangle(Polygon3(nrm, vx_1, vx_3, vx_4))

        if msg.z2 != msg.z0:
            vx_1 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            vx_2 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           msg.z2)
            vx_3 = Vertex3(msg.x * self._density,
                           (msg.y + 1) * self._density,
                           msg.z2)
            vx_4 = Vertex3((msg.x + 1) * self._density,
                           (msg.y + 1) * self._density,
                           msg.z0)
            nrm = Vertex3(0.0, 1.0, 0.0)
            if msg.z2 >= msg.z0:
                nrm.y = -1.0

            self._file.add_triangle(Polygon3(nrm, vx_1, vx_2, vx_3))
            self._file.add_triangle(Polygon3(nrm, vx_1, vx_4, vx_2))

        if (msg.y == 0) and (msg.z0 > 0):
            vx_1 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_2 = Vertex3(msg.x * self._density,
                           msg.y * self._density,
                           0.0)
            vx_3 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           msg.z0)
            vx_4 = Vertex3((msg.x + 1) * self._density,
                           msg.y * self._density,
                           0.0)
            nrm = Vertex3(0.0, -1.0, 0.0)

            self._file.add_triangle(Polygon3(nrm, vx_1, vx_2, vx_3))
            self._file.add_triangle(Polygon3(nrm, vx_2, vx_4, vx_3))
