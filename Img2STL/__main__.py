import wx
from .ui import Img2STLMainFrame


app = wx.App(False)
frame = Img2STLMainFrame()
frame.SetIcon(frame.app_icon)
frame.Show(True)
app.MainLoop()
