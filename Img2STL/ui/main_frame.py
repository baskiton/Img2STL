import os
import wx
import wx.propgrid as wx_pg

from .ui import MainFrame
from .about_dialog import Img2STLAboutDialog
from ..static import *
from ..utils import get_bound_box, ThrCmds, ThreadEvent, Logging, Executor


def wxbuildinfo():
    wx_buld = wx.VERSION_STRING
    platform = wx.PlatformInformation()
    os_name = platform.GetOperatingSystemFamilyName()

    return f"wxPython-{wx_buld}-{os_name}"


class Img2STLMainFrame(MainFrame):
    def __init__(self, title: str = "Img2STL"):
        super().__init__(title=title)
        # self.Title = title

        self._files_directory = os.getcwd()
        self._exit_files_dir = os.getcwd()
        self._files_list = []
        self._exit_files = []

        self._logger = Logging("log.txt")
        self._executor: Executor = None

        self._cnt = False
        self.app_icon = wx.Icon(APP_ICON)

        self.Fit()

        self.Bind(wx.EVT_THREAD, self.OnThreadUpdate)
        wx.LogMessage("The program is running.")

    def OnThreadUpdate(self, event: ThreadEvent) -> None:
        ev_id: ThrCmds = event.GetId()

        if ev_id == ThrCmds.thrCMD_POOL_END:
            self.m_progress_label.SetLabel("Done!")
            self.m_progress_label.CenterOnParent(wx.HORIZONTAL)
            self.m_prop_panel.Enable(True)
            self.m_progress_bar.SetValue(100)
            self._executor.join()
            del self._executor
            self._executor = None

        elif ev_id == ThrCmds.thrCMD_POOL_POINT:
            self.m_progress_label.SetLabel(event.data)
            self.m_progress_label.CenterOnParent(wx.HORIZONTAL)

        elif ev_id == ThrCmds.thrCMD_PBAR_UPD:
            self.m_progress_bar.SetValue(event.data)

    def set_prop_exit_label(self) -> None:
        fn = self._exit_files_dir
        cnt = len(self._files_list)

        if cnt == 0:
            return
        elif cnt == 1:
            fn = os.path.join(fn, self._exit_files[0])

        self.m_prop_exit_label.SetLabel(fn)
        self.m_prop_exit_label.SetToolTip(fn)

    def add_image_tabs(self) -> None:
        if len(self._files_list) == 0:
            return

        self.m_note.DeleteAllPages()

        for item in self._files_list:
            tab = wx.Panel(self)
            self.m_note.AddPage(tab, item)

    def change_image_scale(self) -> None:
        if len(self._files_list) == 0:
            return

        cont_size: wx.Size = self.m_image_container.GetSize()
        cont_ratio = cont_size.GetWidth() / cont_size.GetHeight()

        fn = os.path.join(self._files_directory, self.m_note.GetPageText(self.m_note.GetSelection()))

        img = wx.Image(fn)
        if not img.IsOk():
            return

        if self.m_prop_image_autocrop.GetValue():
            bbox = get_bound_box(img, self.m_prop_mask_mode_val.GetSelection(),
                                 self.m_prop_transp_col_val.GetBackgroundColour())

            if bbox:
                img.Resize(bbox[0], bbox[1])

        if self.m_prop_mask_mode_val.GetSelection():
            col: wx.Colour = self.m_prop_transp_col_val.GetBackgroundColour()
            img.SetMaskColour(col.Red(), col.Green(), col.Blue())

        if not img.HasAlpha():
            img.InitAlpha()

        self.fill_palette(img)

        img_ratio = img.GetWidth() / img.GetHeight()

        if cont_ratio > img_ratio:
            w = (img.GetWidth() * cont_size.GetHeight()) / img.GetHeight() * 0.8
            h = cont_size.GetHeight() * 0.8
        else:
            w = cont_size.GetWidth() * 0.8
            h = (img.GetHeight() * cont_size.GetWidth()) / img.GetWidth() * 0.8

        img.Rescale(w, h)

        tab: wx.Panel = self.m_note.GetCurrentPage()
        tab.DestroyChildren()

        bm = wx.StaticBitmap(tab, bitmap=wx.Bitmap(img))
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(bm, 1, wx.ALL | wx.EXPAND, 5)

        del img

        tab.SetSizer(sizer)
        tab.Layout()
        sizer.Fit(tab)
        self.m_note.Layout()

    def fill_palette(self, img: wx.Image) -> None:
        table = dict()
        data: bytearray = img.GetData()
        alpha: bytearray = img.GetAlpha()

        for i in range(img.GetHeight() * img.GetWidth()):
            if alpha[i] == 0:
                continue
            idx = i*3
            r = data[idx]
            g = data[idx+1]
            b = data[idx+2]
            rgb = (r, g, b)
            if rgb in table:
                table[rgb][0] += 1
            else:
                table[rgb]=[1, wx.Colour(r, g, b)]

        self.m_palette_grid.Clear()
        self.m_palette_grid.Append(wx_pg.PropertyCategory("amount | color"))

        for i in table.values():
            self.m_palette_grid.Append(wx_pg.ColourProperty(str(i[0]), "", i[1]))

        self.m_palette_grid.SetSplitterPosition(50)

    def OnClose(self, event) -> None:
        wx.LogMessage("Closing the program.")

        # Executor will be terminated automatically because it is a daemon

        self.Destroy()

    def OnExit(self, event) -> None:
        self.Close()

    def OnAbout(self, event) -> None:
        info = Img2STLAboutDialog(self)

        info.set_icon(self.app_icon)
        info.set_name(ST_NAME, ST_VERSION)
        info.set_description(ST_DESCRIPTION)
        info.set_using("using " + wxbuildinfo())
        info.set_license(ST_LICENSE)
        info.set_copyright(ST_LICENSE_LINK, ST_COPYRIGHT)
        info.add_link("https://github.com/baskiton/Img2STL", "Homapage: GitHub")

        info.fit()
        info.ShowModal()
        info.Destroy()

    def main_containerOnSplitterSashPosChanging(self, event) -> None:
        self.Layout()

    def prop_image_openOnButtonClick(self, event) -> None:
        open_file_dlg = wx.FileDialog(
            self,
            "Open image files",
            self._files_directory,
            wx.EmptyString,
            f"Images file (*.bmp *.gif *.png *.jpg)|*.bmp;*.gif;*.png;*.jpg|"   \
            f"All files ({wx.FileSelectorDefaultWildcardStr})|" \
            f"{wx.FileSelectorDefaultWildcardStr}",
            wx.FD_OPEN | wx.FD_MULTIPLE | wx.FD_CHANGE_DIR
        )

        if open_file_dlg.ShowModal() == wx.ID_CANCEL:
            return

        self._files_directory = open_file_dlg.GetDirectory()
        self._exit_files_dir = self._files_directory
        self._files_list = open_file_dlg.GetFilenames()
        self._exit_files = open_file_dlg.GetFilenames()

        names = "; ".join(self._exit_files)
        for item in self._exit_files:
            fn, _ = os.path.splitext(item)
            item = f"{fn}.stl"

        wx.LogMessage(f"Files {names} opened.")

        self.set_prop_exit_label()
        self.add_image_tabs()

        self.m_execute_button.Enable(True)
        self.m_prop_exit_btn.Enable(True)

    def prop_image_autocropOnCheckBox(self, event) -> None:
        self.change_image_scale()

    def prop_mask_mode_valOnChoice(self, event) -> None:
        val = self.m_prop_mask_mode_val.GetSelection()

        self.m_prop_transp_col_lable.Enable(val)
        self.m_prop_transp_col_val.Enable(val)

        self.change_image_scale()

    def prop_transp_col_valOnButtonClick(self, event) -> None:
        col = wx.ColourData()
        col.SetColour(self.m_prop_transp_col_val.GetBackgroundColour())
        col_dlg = wx.ColourDialog(self, col)

        if col_dlg.ShowModal() == wx.ID_CANCEL:
            return

        col.SetColour(col_dlg.GetColourData().GetColour())
        self.m_prop_transp_col_val.SetBackgroundColour(col.GetColour())
        self.m_prop_transp_col_val.SetLabel(col.GetColour().GetAsString(wx.C2S_HTML_SYNTAX))

        self.change_image_scale()

    def prop_exit_btnOnButtonClick(self, event) -> None:
        count = len(self._files_list)

        if count == 1:      # single file
            save_file_dlg = wx.FileDialog(
                self,
                "Save File",
                self._exit_files_dir,
                self._exit_files[0],
                f"STL file (*.stl)|*.stl|"  \
                f"All files ({wx.FileSelectorDefaultWildcardStr})"
                f"|{wx.FileSelectorDefaultWildcardStr}",
                wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
            )
            if save_file_dlg.ShowModal() == wx.ID_CANCEL:
                return
            self._exit_files_dir = save_file_dlg.GetDirectory()

        elif count > 1:     # multi files
            save_dir_dlg = wx.DirDialog(
                self,
                "Save to directory",
                self._exit_files_dir
            )
            if save_dir_dlg.ShowModal() == wx.ID_CANCEL:
                return
            self._exit_files_dir = save_dir_dlg.GetPath()

        wx.LogMessage(f"Exit directory changed to {self._exit_files_dir}")

        self.set_prop_exit_label()

    def execute_buttonOnButtonClick(self, event) -> None:
        self.m_progress_bar.SetValue(0)

        # TODO
        self._executor = Executor(
            self
        )

        self._executor.start()

        self.m_prop_panel.Enable(False)
        self.m_progress_label.SetLabel("Process...")
        self.m_progress_label.CentreOnParent(wx.HORIZONTAL)

    def image_panel_containerOnSize(self, event: wx.SizeEvent) -> None:
        # FIXME
        # I don’t understand why yet, but this method is called twice,
        # so this is where the "protection" is organized using a counter

        if self._cnt:
            self._cnt = False
            self.m_image_panel_container.SetSashPosition(event.GetSize().GetWidth() - 200)
            self.change_image_scale()
        else:
            self._cnt = True

    def image_panel_containerOnSplitterSashPosChanged(self, event) -> None:
        self.m_image_info_panel.SetSize(200, -1)

    def noteOnAuiNotebookPageChanged(self, event) -> None:
        # TODO
        # if self.m_note.GetClientData():
        #     return

        self.change_image_scale()

    def noteOnAuiNotebookPageClose(self, event) -> None:
        if self.m_note.GetPageCount() <= 0:
            self.m_palette_grid.Clear()
            self.m_palette_grid.Append(wx_pg.PropertyCategory("amount | color"))
