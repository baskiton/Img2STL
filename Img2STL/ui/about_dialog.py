import wx
import wx.adv

from .ui import AboutDialog


class Img2STLAboutDialog(AboutDialog):
    def __init__(self, parent):
        super().__init__(parent)

    def fit(self) -> None:
        self.Layout()
        self.m_sizer_main.Fit(self)

    def set_icon(self, icon: wx.Icon) -> None:
        self.m_app_icon.SetIcon(icon)

    def set_name(self, name: str, version: str) -> None:
        self.m_name_version.SetLabel(f"{name} {version}")

    def set_description(self, descr: str) -> None:
        self.m_description.SetLabel(descr)

    def set_using(self, using: str) -> None:
        self.m_using_str.SetLabel(using)

    def set_license(self, license: str) -> None:
        pane: wx.Window = self.m_license_pane.GetPane()
        lic = wx.TextCtrl(pane, value=license,
                          style=wx.TE_BESTWRAP | wx.TE_MULTILINE | wx.TE_READONLY | wx.TE_WORDWRAP)

        self.m_sizer_license.Add(lic, 1, wx.EXPAND, 5)

        pane.SetSizer(self.m_sizer_license)
        pane.Layout()
        self.m_sizer_license.Fit(pane)

    def set_copyright(self, url: str, label: str) -> None:
        self.m_copyright.SetURL(url)
        self.m_copyright.SetLabel(label)

    def add_link(self, url: str, label: str) -> None:
        link = wx.adv.HyperlinkCtrl(self, label=label, url=url)
        self.m_url_sizer.Add(link, 0, wx.ALL, 5)
