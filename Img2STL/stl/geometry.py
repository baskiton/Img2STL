import struct


class Vertex3(struct.Struct):
    def __init__(self, x: float, y: float, z: float):
        super().__init__("3f")
        self.x = x
        self.y = y
        self.z = z

    def pack(self) -> bytes:
        return super().pack(self.x, self.y, self.z)

    def __str__(self):
        return f"{self.x:e} {self.y:e} {self.z:e}"


class Polygon3():
    def __init__(self, nrm, vx_1: Vertex3, vx_2: Vertex3, vx_3: Vertex3):
        self.nrm = nrm
        self.vx_1 = vx_1
        self.vx_2 = vx_2
        self.vx_3 = vx_3

    def to_bytes(self) -> bytes:
        return self.nrm.pack() + self.vx_1.pack() + self.vx_2.pack() + self.vx_3.pack() + b"\0\0"
