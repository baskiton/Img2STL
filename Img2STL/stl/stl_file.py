from __future__ import annotations

import os
import wx
import struct

from typing import List
from .geometry import Polygon3


class STLFile:
    ASCII = 0
    BIN = 1

    def __init__(self, f_name: str = "") -> None:
        self._name = f"{f_name:80}"
        if len(self._name) > 80:
            self._name = self._name[:80]
        self._polygons_list: List[Polygon3] = []

    def add_triangle(self, polygon: Polygon3) -> None:
        self._polygons_list.append(polygon)

    def add_stl(self, stl: STLFile) -> None:
        self._polygons_list.extend(stl._polygons_list)

    def clear(self) -> None:
        self._polygons_list.clear()

    def set_header(self, hdr: str) -> None:
        self._name = f"{hdr:80}"
        if len(self._name) > 80:
            self._name = self._name[:80]

    def save_file(self, f_type, f_name: str) -> bool:
        mode = "w" if f_type == STLFile.ASCII else "wb"

        with open(f_name, mode) as file:
            result = self.to_ascii() if f_type == STLFile.ASCII else self.to_bin()
            file.write(result)

        wx.LogMessage(f"File {os.path.basename(f_name)} created.")

        return True

    def polygons(self) -> int:
        return len(self._polygons_list)

    def to_ascii(self) -> str:
        result = "solid {}\n".format(self._name.replace(" \n\t", '_'))

        for polygon in self._polygons_list:
            result += f" facet normal {polygon.nrm}\n"  \
                      f"  outer loop\n"                 \
                      f"   vertex {polygon.vx_1}\n"     \
                      f"   vertex {polygon.vx_2}\n"     \
                      f"   vertex {polygon.vx_3}\n"     \
                      f"  endloop\n endfacet\n"

        result += f"endsolid {self._name}"

        return result

    def to_bin(self) -> bytearray:
        result = bytearray()

        result += struct.pack("80s", bytes(self._name, "ascii"))    # header
        result += struct.pack("I", self.polygons())
        for polygon in self._polygons_list:
            result += polygon.to_bytes()

        return result
