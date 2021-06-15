from Img2STL.utils.cmd import WMCmds
from Img2STL.utils.utils import WorkerMessage
import os
import wx

from threading import Thread
from multiprocessing import Process, Queue
from typing import List

from .cmd import ThrCmds
from .utils import ThreadEvent, get_bound_box, img_to_heightmap
from .worker import Worker
from ..stl import STLFile

from time import sleep


class Executor(Thread):
    def __init__(self,
                 evt_hdlr: wx.EvtHandler,
                 autocrop: bool,
                 density: float,
                 height_max: float,
                 height_min: float,
                 mask_mode: bool,
                 mask_color: wx.Colour,
                 height_mode: bool,
                 f_type: STLFile.BIN or STLFile.ASCII,
                 files_dir: str,
                 files_list: List[str],
                 exit_files_dir: str,
                 exit_files_list: List[str]) -> None:
        super().__init__(name="Executor", daemon=True)
        self._autocrop = autocrop
        self._density = density
        self._height_max = height_max
        self._height_min = height_min
        self._mask_mode = mask_mode
        self._mask_color = mask_color
        self._height_mode = height_mode
        self._f_type = f_type

        self._files_dir = files_dir
        self._files_list = files_list
        self._exit_files_dir = exit_files_dir
        self._exit_files_list = exit_files_list

        self._event = ThreadEvent()
        self._main_evt_handler = evt_hdlr
        self._worker_pool: List[Process] = []
        self._q_to_worker = Queue()
        self._q_from_worker = Queue()

        wx.LogMessage("Executor created.")

    # def __del__(self) -> None:
    #     # self._main_evt_handler._executor = None
    #     wx.LogMessage("Executor destroyed.")

    def _finish(self):
        self._event.SetId(ThrCmds.thrCMD_POOL_END)
        wx.QueueEvent(self._main_evt_handler, self._event.Clone())
        wx.LogMessage("Executor is stopped.")

    def run(self) -> None:
        wx.LogMessage("Executor is running.")

        if len(self._files_list) == 0:
            self._finish()
            return

        points_per_file = 100.0 / len(self._files_list)
        pb_val = 0.0
        cpu_cnt = os.cpu_count()
        if cpu_cnt <= 0:
            cpu_cnt = 1

        mod = self._height_min if self._height_mode else self._height_max
        height_factor = 255.0 / (self._height_max - self._height_min)

        if not self._height_mode:
            height_factor *= -1

        for idx, img_name in enumerate(self._files_list):
            wx.LogMessage(f"Executing for {img_name}")

            self._event.SetId(ThrCmds.thrCMD_POOL_POINT)
            self._event.data = img_name
            wx.QueueEvent(self._main_evt_handler, self._event.Clone())

            for i in range(cpu_cnt):
                proc = Worker(i, self._density, self._q_to_worker, self._q_from_worker)
                self._worker_pool.append(proc)
                proc.start()

            if len(self._worker_pool) == 0:
                wx.LogError("Worker Pool is empty!")
                break

            bw = wx.Image(os.path.join(self._files_dir, img_name))

            if self._autocrop:
                bbox = get_bound_box(bw, self._mask_mode, self._mask_color)
                if bbox:
                    bw.Resize(bbox[0], bbox[1])

            bw = img_to_heightmap(bw, self._mask_mode, self._mask_color)

            if not bw.HasAlpha():
                bw.InitAlpha()

            data: bytearray = bw.GetData()
            alpha: bytearray = bw.GetAlpha()
            img_h = bw.GetHeight()
            img_w = bw.GetWidth()
            tot_count = len(alpha)

            for y in range(img_h):
                for x in range(img_w):
                    z0 = z1 = z2 = 0
                    if alpha[(img_w * y) + x]:
                        z0 = data[((img_w * y) + x) * 3] / height_factor + mod
                    if ((x + 1) < img_w) and alpha[(img_w * y) + x + 1]:
                        z1 = data[((img_w * y) + x + 1) * 3] / height_factor + mod
                    if ((y - 1) >= 0) and alpha[(img_w * (y - 1)) + x]:
                        z2 = data[((img_w * (y - 1)) + x) * 3] / height_factor + mod

                    self._q_to_worker.put(
                        WorkerMessage(WMCmds.wMSG_RUN, x, img_h - y - 1, z0, z1, z2))

            for _ in self._worker_pool:
                self._q_to_worker.put(WorkerMessage(WMCmds.wMSG_END))

            while True:
                i = self._q_to_worker.qsize()
                val = pb_val + points_per_file * (1 - (i / tot_count))
                self._event.SetId(ThrCmds.thrCMD_PBAR_UPD)
                self._event.data = int(val)
                wx.QueueEvent(self._main_evt_handler, self._event.Clone())
                if i == 0:
                    pb_val += points_per_file
                    break
                sleep(0.1)

            # FIXME: processes do not terminated
            # for proc in self._worker_pool:
            #     proc.join()
            while self._q_from_worker.qsize() != len(self._worker_pool):
                sleep(0.05)

            self._worker_pool.clear()
            self._generate_stl(idx)

        self._finish()

    def _generate_stl(self, idx: int):
        f_name = self._exit_files_list[idx]
        result_file = STLFile(f_name)

        if self._f_type == STLFile.BIN:
            result_file.set_header(
                f"Generate from Img2STL by baskiton. {f_name}")

        for _ in range(self._q_from_worker.qsize()):
            stl: STLFile = self._q_from_worker.get()
            result_file.add_stl(stl)

        result_file.save_file(self._f_type,
                              os.path.join(self._exit_files_dir, f_name))
