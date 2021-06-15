import wx

from .cmd import WMCmds
from typing import Tuple


class ThreadEvent(wx.PyEvent):
    def __init__(self, id=0) -> None:
        super().__init__(id=id, eventType=wx.wxEVT_THREAD)


class WorkerMessage:
    def __init__(self, cmd: WMCmds, x: int = None, y: int = None,
                 z0: float = None, z1: float = None, z2: float = None) -> None:
        self._cmd = cmd
        self._x = x
        self._y = y
        self._z0 = z0
        self._z1 = z1
        self._z2 = z2

    @property
    def cmd(self) -> WMCmds:
        return self._cmd

    @property
    def x(self) -> int:
        return self._x

    @property
    def y(self) -> int:
        return self._y

    @property
    def z0(self) -> float:
        return self._z0

    @property
    def z1(self) -> float:
        return self._z1

    @property
    def z2(self) -> float:
        return self._z2


def get_bound_box(img: wx.Image, mode: bool, color: wx.Colour) -> Tuple[wx.Size, wx.Point]:
    img_h = img.GetHeight()
    img_w = img.GetWidth()
    bbox = [img_w, -1, 0, 0]
    mask_v = bytearray((color.Red(), color.Green(), color.Blue()))
    data = img.GetData()
    alpha = img.GetAlpha()

    for y in range(img_h):
        has_data = False
        yt = img_w * y
        for x in range(img_w):
            xt = (yt + x) * 3
            if ((not mode and alpha and alpha[yt+x]) or
                (mode and (data[xt:xt+3] != mask_v))):
                has_data = True
                if x < bbox[0]:
                    bbox[0] = x
                if x >= bbox[2]:
                    bbox[2] = x + 1

        if has_data:
            if bbox[1] < 0:
                bbox[1] = y
            bbox[3] = y + 1

    if bbox[1] < 0:
        return tuple()

    return (wx.Size(bbox[2] - bbox[0], bbox[3] - bbox[1]),
            wx.Point(-bbox[0], -bbox[1]))


def img_to_heightmap(img: wx.Image, mode: bool, color: wx.Colour) -> wx.Image:
    if mode:
        img.SetMaskColour(color.Red(), color.Green(), color.Blue())

    return wx.Image(img.ConvertToGreyscale())
