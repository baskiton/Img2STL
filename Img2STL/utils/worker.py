import wx

from multiprocessing import Process, Pipe
from multiprocessing.connection import Connection
from ..stl import Vertex3, Polygon3, STLFile
from ..utils import WorkerMessage, WMCmds


class Worker(Process):
    def __init__(self, tid: int, den: float, pipe: Connection) -> None:
        super().__init__(name=f"Worker-{tid}", daemon=True)
        self._density = den
        self._file = STLFile()
        self._pipe = pipe

        wx.LogMessage(f"{self.name} created.")

    def run(self) -> None:
        wx.LogMessage(f"{self.name} is running.")

        while True:
            msg: WorkerMessage = self._pipe.recv()

            if msg.cmd == WMCmds.wMSG_RUN:
                self._pixel_calc(msg)
            elif msg.cmd == WMCmds.wMSG_END:
                self._pipe.send(self._file)
                break
            elif msg.cmd == WMCmds.wMSG_STOP:
                break

        wx.LogMessage(f"{self.name} is stopped.")

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

            vx_1.z = vx_2.z = vx_3.z = vx_4.z = 0.0
            nrm.z = -1.0

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
