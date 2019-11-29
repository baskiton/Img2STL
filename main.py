#!/usr/bin/python3
# -*- coding: utf-8 -*-

import wx
import wx.aui
import wx.propgrid as pg
from threading import Thread, Lock
from multiprocessing import Pool, Pipe
from time import sleep
from PIL import Image
import struct
import re
import os
from colorsys import rgb_to_hsv


nb_page_changed = wx.aui.EVT_AUINOTEBOOK_PAGE_CHANGED.typeId
nb_page_closed = wx.aui.EVT_AUINOTEBOOK_PAGE_CLOSED.typeId

EVT_PROCESS_ID = wx.ID_ANY


class Converter:
    def __init__(self, density, max_h, min_h, exit_type, mode, h_mode,
                 mask_mode, color, image_path, image_file,
                 save_path, save_file, auto_crop, pipe):
        if h_mode == 'Black':
            h_mode = False
        else:
            h_mode = True

        self.density = density       # mm per pixel
        self.max_height = max_h     # mm
        self.min_height = min_h     # mm
        self.exit_type = exit_type  # 'ASCII' or 'BIN'
        self.mode = mode            # 'Pixel' or 'Image'
        self.white_is_height = h_mode
        self.height_factor = 255 / (self.max_height - self.min_height)
        self.mask_mode = mask_mode  # 'Transparency' or 'Color'
        self.transparency_color = color     # tuple
        self.auto_crop = auto_crop
        self.pipe = pipe

        self.image_path = image_path
        self.image_file = image_file

        self.img_name = re.search(r'^(.+)\.\w{1,4}$', image_file).group(1)

        self.img, self.bw = self.image_open_convert()

        self.save_path = save_path
        self.save_file = save_file

        self.result_ascii = f'solid {self.img_name}\n'
        self.result_bin = (b'Exported from un'
                           b'named program by'
                           b' baskiton' + (b'\x00' * 43))

        self.triangles_count = 0

        self.number_of_pixels = self.bw.height * self.bw.width

    def exec(self):
        if self.exit_type == 'ASCII':
            body = self.body_stl_ascii
        else:
            body = self.body_stl_bin

        self.process(body)
        self.save_stl()

    def get_number_of_pixels(self):
        return self.number_of_pixels

    def image_open_convert(self):
        with Image.open(os.path.join(self.image_path, self.image_file)) as img:
            if self.auto_crop:
                img = img.crop(img.getbbox())

        bw = img.convert('LA')
        if self.mask_mode == 'Color':
            for y in range(img.height):
                for x in range(img.width):
                    if img.getpixel((x, y)) == self.transparency_color:
                        bw.putpixel((x, y), (0, 0))

        img = img.transpose(Image.FLIP_TOP_BOTTOM)
        bw = bw.transpose(Image.FLIP_TOP_BOTTOM)
        return img, bw

    @staticmethod
    def body_stl_ascii(nrm, polygon):
        result_ascii = ''
        vx_1, vx_2, vx_3 = polygon

        result_ascii += f' facet normal {" ".join(map(str, nrm))}\n'
        result_ascii += '  outer loop\n'
        result_ascii += f'   vertex {" ".join(map(str, vx_1))}\n'
        result_ascii += f'   vertex {" ".join(map(str, vx_2))}\n'
        result_ascii += f'   vertex {" ".join(map(str, vx_3))}\n'
        result_ascii += '  endloop\n endfacet\n'

        return result_ascii

    @staticmethod
    def body_stl_bin(nrm, polygon):
        result_bin = b''
        vx_1, vx_2, vx_3 = polygon

        result_bin += struct.pack('fff', *nrm)
        result_bin += struct.pack('fff', *vx_1)
        result_bin += struct.pack('fff', *vx_2)
        result_bin += struct.pack('fff', *vx_3)
        result_bin += b'\x00\x00'

        return result_bin

    def calculate_pixel(self, x, y, body, pipe, result):
        triangles_count = 0

        col, alpha = self.bw.getpixel((x, y))
        if alpha == 0:
            z = 0
        elif self.white_is_height:
            z = col / self.height_factor + self.min_height
        else:
            z = col / -self.height_factor + self.max_height

        if alpha != 0:     # if not transparency
            cd_1 = [x * self.density,
                    y * self.density,
                    z]
            cd_2 = [(x + 1) * self.density,
                    y * self.density,
                    z]
            cd_3 = [x * self.density,
                    (y + 1) * self.density,
                    z]
            cd_4 = [(x + 1) * self.density,
                    (y + 1) * self.density,
                    z]
            normal = [0, 0, 1000]
            result += body(normal, (cd_1, cd_2, cd_3))
            result += body(normal, (cd_3, cd_2, cd_4))

            cd_1 = [x * self.density,
                    y * self.density,
                    0]
            cd_2 = [x * self.density,
                    (y + 1) * self.density,
                    0]
            cd_3 = [(x + 1) * self.density,
                    y * self.density,
                    0]
            cd_4 = [(x + 1) * self.density,
                    (y + 1) * self.density,
                    0]
            normal = [0, 0, -1000]
            result += body(normal, (cd_1, cd_2, cd_3))
            result += body(normal, (cd_2, cd_4, cd_3))

            triangles_count += 4

        col_side = 0
        alpha_side = 0
        try:
            col_side, alpha_side = self.bw.getpixel((x + 1, y))
        except IndexError:
            col_side = 0
            alpha_side = 0
        finally:
            if alpha_side == 0:
                z_side = 0
            elif self.white_is_height:
                z_side = col_side / self.height_factor + self.min_height
            else:
                z_side = col_side / -self.height_factor + self.max_height

            if z_side != z:
                if z_side < z:
                    normal = [1000, 0, 0]
                else:
                    normal = [-1000, 0, 0]

                cd_1 = [(x + 1) * self.density,
                        y * self.density,
                        z]
                cd_2 = [(x + 1) * self.density,
                        y * self.density,
                        z_side]
                cd_3 = [(x + 1) * self.density,
                        (y + 1) * self.density,
                        z]
                cd_4 = [(x + 1) * self.density,
                        (y + 1) * self.density,
                        z_side]

                result += body(normal, (cd_1, cd_2, cd_3))
                result += body(normal, (cd_2, cd_4, cd_3))

                triangles_count += 2

            if x == 0 and z > 0:
                normal = [-1000, 0, 0]

                cd_1 = [x * self.density,
                        y * self.density,
                        z]
                cd_2 = [x * self.density,
                        (y + 1) * self.density,
                        z]
                cd_3 = [x * self.density,
                        (y + 1) * self.density,
                        0]
                cd_4 = [x * self.density,
                        y * self.density,
                        0]

                result += body(normal, (cd_1, cd_2, cd_3))
                result += body(normal, (cd_1, cd_3, cd_4))

                triangles_count += 2

        col_side = 0
        alpha_side = 0
        try:
            col_side, alpha_side = self.bw.getpixel((x, y + 1))
        except IndexError:
            col_side = 0
            alpha_side = 0
        finally:
            if alpha_side == 0:
                z_side = 0
            elif self.white_is_height:
                z_side = col_side / self.height_factor + self.min_height
            else:
                z_side = col_side / -self.height_factor + self.max_height

            if z_side != z:
                if z_side < z:
                    normal = [0, 1000, 0]
                else:
                    normal = [0, -1000, 0]

                cd_1 = [x * self.density,
                        (y + 1) * self.density,
                        z]
                cd_2 = [(x + 1) * self.density,
                        (y + 1) * self.density,
                        z_side]
                cd_3 = [x * self.density,
                        (y + 1) * self.density,
                        z_side]
                cd_4 = [(x + 1) * self.density,
                        (y + 1) * self.density,
                        z]

                result += body(normal, (cd_1, cd_2, cd_3))
                result += body(normal, (cd_1, cd_4, cd_2))

                triangles_count += 2

            if y == 0 and z > 0:
                normal = [0, -1000, 0]

                cd_1 = [x * self.density,
                        y * self.density,
                        z]
                cd_2 = [x * self.density,
                        y * self.density,
                        0]
                cd_3 = [(x + 1) * self.density,
                        y * self.density,
                        z]
                cd_4 = [(x + 1) * self.density,
                        y * self.density,
                        0]

                result += body(normal, (cd_1, cd_2, cd_3))
                result += body(normal, (cd_2, cd_4, cd_3))

                triangles_count += 2

        pipe.send(1)

        return result, triangles_count

    def process(self, body):
        self.result_ascii = f'solid {self.img_name}\n'
        self.result_bin = (b'Exported from un'
                           b'named program by'
                           b' baskiton' + (b'\x00' * 43))

        if self.exit_type == 'ASCII':
            pre_result = ''
        else:
            pre_result = b''

        params = []
        for y in range(self.bw.height):
            for x in range(self.bw.width):
                params.append((x, y, body, self.pipe, pre_result))

        with Pool() as worker:
            result = worker.starmap(self.calculate_pixel, params)

        if self.exit_type == 'ASCII':
            self.result_ascii += ''.join([i[0] for i in result])
        else:
            self.result_bin += b''.join([i[0] for i in result])
            self.triangles_count = sum([i[1] for i in result])

    def save_stl(self):
        if self.exit_type == 'ASCII':
            self.result_ascii += f'endsolid {self.img_name}'
            with open(os.path.join(self.save_path, self.save_file),
                      'w') as stl:
                stl.write(self.result_ascii)

        elif self.exit_type == 'BIN':
            count = struct.pack('I', self.triangles_count)
            self.result_bin = bytearray(self.result_bin)
            self.result_bin[80:84] = count
            self.result_bin = bytes(self.result_bin)

            with open(os.path.join(self.save_path, self.save_file),
                      'wb') as stl:
                stl.write(self.result_bin)


def evt_process(win, func):
    """Result Event Definition"""
    win.Connect(-1, -1, EVT_PROCESS_ID, func)


class ProcessEvent(wx.PyEvent):
    """A simple event containing various result data"""
    def __init__(self, data):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_PROCESS_ID)
        self.data = data


class ConverterThread(Thread):
    def __init__(self, wx_object, kwargs):
        Thread.__init__(self)
        self.wx_object = wx_object

        self.number_of_pixels = 0
        self.counter = 0
        self.to_stl = []

        self.parent_pipe, self.child_pipe = Pipe()

        self.prepare(**kwargs, pipe=self.child_pipe)

        wx.PostEvent(self.wx_object,
                     ProcessEvent(('init', self.number_of_pixels)))

    def listing(self):
        for i in range(self.number_of_pixels):
            self.counter += self.parent_pipe.recv()

    def prepare(self, density, max_h, min_h, exit_type, mode, h_mode,
                mask_mode, color, image_path, image_files,
                save_path, save_files, auto_crop, pipe):
        for idx, image in enumerate(image_files):
            file = Converter(density, max_h, min_h, exit_type, mode, h_mode,
                             mask_mode, color, image_path, image,
                             save_path, save_files[idx], auto_crop, pipe)
            self.to_stl.append(file)
            self.number_of_pixels += file.get_number_of_pixels()

    def run(self):
        self.counter = 0
        Thread(target=self.listing).start()

        for idx in range(len(self.to_stl)):
            file = self.to_stl.pop(0)
            wx.PostEvent(self.wx_object,
                         ProcessEvent(('name', file.image_file)))
            file.exec()
            del file

        wx.PostEvent(self.wx_object, ProcessEvent(('end', 100)))


class UpdaterThread(Thread):
    def __init__(self, wx_object, mutex):
        Thread.__init__(self)
        self.wx_object = wx_object
        self.mutex = mutex

    def run(self):
        numb_pix = app.frame.converter.number_of_pixels
        while True:
            with self.mutex:
                count = app.frame.converter.counter
            if count >= numb_pix:
                break
            wx.PostEvent(self.wx_object, ProcessEvent(('count', count)))
            sleep(0.1)


class MyFrame(wx.Frame):
    def __init__(self, parent=None):
        wx.Frame.__init__(
            self, parent, id=wx.ID_ANY, title='Image to STL',
            pos=wx.DefaultPosition, size=wx.Size(800, 600),
            style=wx.DEFAULT_FRAME_STYLE | wx.MAXIMIZE | wx.TAB_TRAVERSAL
        )
        self.font = wx.Font(9, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL,
                            wx.FONTWEIGHT_NORMAL, False, "Noto Sans")

        self.SetFont(self.font)
        self.open_file_flag = False

        self.files_list = dict()
        self.files_directory = os.getcwd()
        self.exit_files = []
        self.exit_file_dir = os.getcwd()

        self.transp_colour = wx.Colour(255, 0, 255)
        self.col_data = wx.ColourData()
        self.col_data.SetColour(self.transp_colour)

        self.SetMinSize(wx.Size(500, 300))

        self.menu_bar()

        self.main_sizer = wx.BoxSizer(wx.VERTICAL)

        self.main_container = wx.SplitterWindow(
            self, wx.ID_ANY,
            style=wx.SP_LIVE_UPDATE | wx.SP_NOSASH
        )

        self.prop_panel()
        self.image_panel()
        self.binding_events()

        self.main_container.SplitVertically(self.proppanel, self.imagepanel,
                                            280)

        self.main_sizer.Add(self.main_container, 1, wx.EXPAND, 5)

        self.SetSizer(self.main_sizer)
        self.Layout()

        self.Centre(wx.BOTH)

        evt_process(self, self.status_update)

    def __del__(self):
        pass

    def menu_bar(self):
        menu_bar = wx.MenuBar(0)
        menu_bar.SetFont(self.font)

        menu_file = wx.Menu()
        menu_file.AppendSeparator()

        self.menu_file_exit = wx.MenuItem(menu_file, wx.ID_EXIT,
                                          'Exit\tCtrl-Q')
        menu_file.Append(self.menu_file_exit)

        menu_bar.Append(menu_file, 'File')

        self.SetMenuBar(menu_bar)

    def prop_panel(self):
        self.proppanel = wx.Panel(self.main_container, style=wx.TAB_TRAVERSAL)
        self.proppanel.SetMinSize(wx.Size(200, -1))

        prop_sizer = wx.BoxSizer(wx.VERTICAL)

        prop_image_open_sizer = wx.StaticBoxSizer(
            wx.StaticBox(self.proppanel, label='Open Image'),
            wx.HORIZONTAL
        )

        self.prop_image_open = wx.Button(prop_image_open_sizer.GetStaticBox(),
                                         wx.ID_OPEN,
                                         'Open Image(s)')
        self.prop_image_open.SetDefault()
        prop_image_open_sizer.Add(
            self.prop_image_open,
            1,
            wx.ALL | wx.ALIGN_CENTER_VERTICAL,
            5
        )

        self.prop_image_autocrop = wx.CheckBox(
            prop_image_open_sizer.GetStaticBox(),
            label='Auto Crop'
        )
        self.prop_image_autocrop.SetValue(True)
        prop_image_open_sizer.Add(self.prop_image_autocrop, 0,
                                  wx.ALL | wx.ALIGN_CENTER_VERTICAL, 5)

        prop_sizer.Add(prop_image_open_sizer, 0,
                       wx.EXPAND | wx.RIGHT | wx.LEFT, 5)

        prop_prop_sizer = wx.StaticBoxSizer(
            wx.StaticBox(self.proppanel, label='Property'),
            wx.VERTICAL
        )

        prop_prop_grid = wx.FlexGridSizer(0, 2, 3, 3)
        prop_prop_grid.SetFlexibleDirection(wx.BOTH)
        prop_prop_grid.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)

        self.prop_den_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Density (mm/pix)'
        )

        prop_prop_grid.Add(self.prop_den_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)

        self.prop_den_val = wx.SpinCtrlDouble(
            prop_prop_sizer.GetStaticBox(),
            min=0, max=1000, initial=2, inc=0.001
        )
        self.prop_den_val.SetDigits(3)
        prop_prop_grid.Add(
            self.prop_den_val,
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.EXPAND | wx.RIGHT | wx.LEFT,
            5
        )

        self.prop_max_height_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Max Height (mm)'
        )

        prop_prop_grid.Add(self.prop_max_height_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)

        self.prop_max_height_val = wx.SpinCtrlDouble(
            prop_prop_sizer.GetStaticBox(),
            min=0, max=1000, initial=4, inc=0.001
        )
        self.prop_max_height_val.SetDigits(3)
        prop_prop_grid.Add(
            self.prop_max_height_val,
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.EXPAND | wx.RIGHT | wx.LEFT,
            5
        )

        self.prop_min_height_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Min Height (mm)'
        )

        prop_prop_grid.Add(self.prop_min_height_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)

        self.prop_min_height_val = wx.SpinCtrlDouble(
            prop_prop_sizer.GetStaticBox(),
            min=0, max=1000, initial=1, inc=0.001
        )
        self.prop_min_height_val.SetDigits(3)
        prop_prop_grid.Add(
            self.prop_min_height_val,
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.EXPAND | wx.RIGHT | wx.LEFT,
            5
        )

        self.prop_mask_mode_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Mask Mode'
        )

        prop_prop_grid.Add(self.prop_mask_mode_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)

        prop_mask_mode_valChoices = ['Transparency', 'Color']
        self.prop_mask_mode_val = wx.Choice(
            prop_prop_sizer.GetStaticBox(),
            choices=prop_mask_mode_valChoices
        )
        self.prop_mask_mode_val.SetSelection(0)
        prop_prop_grid.Add(
            self.prop_mask_mode_val,
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.EXPAND | wx.RIGHT | wx.LEFT,
            5
        )

        self.prop_transp_col_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Transparency color'
        )

        if self.prop_mask_mode_val.GetCurrentSelection() == 0:
            self.prop_transp_col_lable.Enable(False)
        prop_prop_grid.Add(self.prop_transp_col_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)


        self.prop_transp_col_val = wx.Button(
            prop_prop_sizer.GetStaticBox(),
            label=self.transp_colour.GetAsString(wx.C2S_HTML_SYNTAX),
            style=wx.BORDER_NONE
        )
        self.prop_transp_col_val.SetBackgroundColour(self.transp_colour)
        if self.prop_mask_mode_val.GetCurrentSelection() == 0:
            self.prop_transp_col_val.Enable(False)
        prop_prop_grid.Add(
            self.prop_transp_col_val,
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.EXPAND | wx.RIGHT | wx.LEFT,
            5
        )

        self.prop_height_mode_lable = wx.StaticText(
            prop_prop_sizer.GetStaticBox(),
            label='Height Mode'
        )

        prop_prop_grid.Add(self.prop_height_mode_lable, 0,
                           wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 5)

        prop_height_mode_valChoices = ['Black', 'White']
        self.prop_height_mode_val = wx.Choice(
            prop_prop_sizer.GetStaticBox(),
            choices=prop_height_mode_valChoices)
        self.prop_height_mode_val.SetSelection(0)
        prop_prop_grid.Add(self.prop_height_mode_val, 0,
                           (wx.ALIGN_CENTER_VERTICAL | wx.EXPAND |
                            wx.RIGHT | wx.LEFT | wx.BOTTOM), 5)

        prop_prop_sizer.Add(prop_prop_grid, 0, 0, 5)

        prop_sizer.Add(prop_prop_sizer, 0,
                       wx.EXPAND | wx.RIGHT | wx.LEFT, 5)

        prop_exit_sizer = wx.StaticBoxSizer(
            wx.StaticBox(self.proppanel, label='Exit File'),
            wx.VERTICAL
        )

        prop_exit_grid = wx.BoxSizer(wx.HORIZONTAL)

        self.prop_exit_label = wx.StaticText(
            prop_exit_sizer.GetStaticBox(),
            label=wx.EmptyString,
            style=wx.ALIGN_LEFT | wx.ST_ELLIPSIZE_MIDDLE | wx.ST_NO_AUTORESIZE
        )

        self.prop_exit_label.SetToolTip(self.prop_exit_label.GetLabel())
        prop_exit_grid.Add(self.prop_exit_label, 1,
                           wx.ALIGN_CENTER_VERTICAL | wx.RIGHT | wx.LEFT, 5)

        self.prop_exit_btn = wx.Button(prop_exit_sizer.GetStaticBox(),
                                       wx.ID_ANY, 'Choose',
                                       wx.DefaultPosition, wx.DefaultSize, 0)
        self.prop_exit_btn.Disable()
        prop_exit_grid.Add(self.prop_exit_btn, 0,
                           wx.LEFT | wx.ALIGN_CENTER_VERTICAL, 5)

        prop_exit_sizer.Add(prop_exit_grid, 0,
                            wx.EXPAND | wx.BOTTOM | wx.RIGHT | wx.LEFT, 5)

        prop_exit_format_choices = ['ASCII', 'BIN']
        self.prop_exit_format = wx.RadioBox(
            prop_exit_sizer.GetStaticBox(),
            label='STL format',
            choices=prop_exit_format_choices
        )
        self.prop_exit_format.SetSelection(1)
        prop_exit_sizer.Add(self.prop_exit_format, 0, wx.LEFT | wx.BOTTOM, 5)

        prop_sizer.Add(prop_exit_sizer, 0,
                       wx.EXPAND | wx.RIGHT | wx.LEFT, 5)

        self.prop_execute_sizer = wx.StaticBoxSizer(
            wx.StaticBox(self.proppanel, label='Execute'),
            wx.VERTICAL
        )

        self.execute_button = wx.Button(self.prop_execute_sizer.GetStaticBox(),
                                        label='Execute')
        self.execute_button.SetDefault()
        self.execute_button.Disable()
        self.prop_execute_sizer.Add(self.execute_button, 0,
                                    wx.ALIGN_CENTER_HORIZONTAL, 5)

        self.progress_bar = wx.Gauge(self.prop_execute_sizer.GetStaticBox(),
                                     range=100,
                                     style=wx.GA_HORIZONTAL | wx.GA_SMOOTH)
        self.progress_bar.SetValue(0)
        self.prop_execute_sizer.Add(self.progress_bar, 0,
                                    wx.EXPAND | wx.ALL, 5)

        self.progress_label = wx.StaticText(
            self.prop_execute_sizer.GetStaticBox()
        )

        self.prop_execute_sizer.Add(self.progress_label, 0,
                                    wx.BOTTOM | wx.ALIGN_CENTER_HORIZONTAL, 5)

        prop_sizer.Add(self.prop_execute_sizer, 0,
                       wx.EXPAND | wx.RIGHT | wx.LEFT | wx.BOTTOM, 5)

        self.proppanel.SetSizer(prop_sizer)
        self.proppanel.Layout()
        prop_sizer.Fit(self.proppanel)

    def image_panel(self):
        self.imagepanel = wx.ScrolledWindow(self.main_container, wx.ID_ANY,
                                            wx.DefaultPosition,
                                            wx.DefaultSize,
                                            wx.HSCROLL | wx.VSCROLL)
        self.imagepanel.SetScrollRate(5, 5)

        image_panel_sizer = wx.BoxSizer(wx.VERTICAL)

        self.image_panel_container = wx.SplitterWindow(
            self.imagepanel,
            wx.ID_ANY,
            wx.DefaultPosition,
            wx.DefaultSize,
            wx.SP_LIVE_UPDATE
        )

        self.image_container = wx.ScrolledWindow(
            self.image_panel_container,
            wx.ID_ANY,
            wx.DefaultPosition,
            wx.DefaultSize,
            wx.HSCROLL | wx.VSCROLL | wx.BORDER_SUNKEN
        )
        self.image_container.SetScrollRate(5, 5)

        self.image_sizer = wx.BoxSizer(wx.VERTICAL)

        self.note = wx.aui.AuiNotebook(self.image_container, wx.ID_ANY,
                                       wx.DefaultPosition, wx.DefaultSize,
                                       (wx.aui.AUI_NB_CLOSE_ON_ALL_TABS |
                                        wx.aui.AUI_NB_SCROLL_BUTTONS |
                                        wx.aui.AUI_NB_TAB_MOVE |
                                        wx.aui.AUI_NB_TAB_SPLIT |
                                        wx.aui.AUI_NB_TOP))
        self.image_sizer.Add(self.note, 1, wx.EXPAND)

        self.image_container.SetSizer(self.image_sizer)
        self.image_container.Layout()
        self.image_sizer.Fit(self.image_container)

        self.image_info_panel = wx.ScrolledWindow(
            self.image_panel_container,
            wx.ID_ANY,
            style=wx.HSCROLL | wx.VSCROLL
        )
        self.image_info_panel.SetScrollRate(5, 5)

        self.image_info_panel_sizer = wx.BoxSizer(wx.VERTICAL)

        self.palette_sizer = wx.StaticBoxSizer(
            wx.StaticBox(self.image_info_panel, wx.ID_ANY, 'Palette'),
            wx.VERTICAL
        )

        self.palette_grid = pg.PropertyGrid(
            self.palette_sizer.GetStaticBox(),
            wx.ID_ANY,
            style=wx.propgrid.PG_LIMITED_EDITING | wx.propgrid.PG_HIDE_MARGIN
        )
        font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL,
                       wx.FONTWEIGHT_NORMAL, False, "Noto Sans")
        self.palette_grid.SetFont(font)

        self.palette_grid.Append(pg.PropertyCategory('amount | color'))

        self.palette_sizer.Add(self.palette_grid, 1, wx.EXPAND, 5)

        self.image_info_panel_sizer.Add(self.palette_sizer, 1, wx.EXPAND, 5)

        self.image_info_panel.SetSizer(self.image_info_panel_sizer)
        self.image_info_panel.Layout()
        self.image_info_panel_sizer.Fit(self.image_info_panel)

        self.image_panel_container.SplitVertically(self.image_container,
                                                   self.image_info_panel, 740)

        image_panel_sizer.Add(self.image_panel_container, 1, wx.EXPAND, 5)

        self.imagepanel.SetSizer(image_panel_sizer)
        self.imagepanel.Layout()
        image_panel_sizer.Fit(self.imagepanel)

    def binding_events(self):
        self.main_container.Bind(wx.EVT_SPLITTER_SASH_POS_CHANGING,
                                 self.events)
        self.image_panel_container.Bind(wx.EVT_SPLITTER_SASH_POS_CHANGING,
                                        self.events)
        self.Bind(wx.EVT_MENU, self.events, id=self.menu_file_exit.GetId())
        self.execute_button.Bind(wx.EVT_BUTTON, self.events)
        self.prop_mask_mode_val.Bind(wx.EVT_CHOICE, self.events)
        self.prop_image_open.Bind(wx.EVT_BUTTON, self.events)
        self.prop_exit_btn.Bind(wx.EVT_BUTTON, self.events)
        self.prop_transp_col_val.Bind(wx.EVT_BUTTON, self.events)
        self.note.Bind(wx.aui.EVT_AUINOTEBOOK_PAGE_CHANGED, self.events)
        self.note.Bind(wx.aui.EVT_AUINOTEBOOK_PAGE_CLOSED, self.events)

    def events(self, event):
        ev_id = event.GetId()

        if ev_id == self.menu_file_exit.GetId():    # exit program
            app.ExitMainLoop()

        elif ev_id == self.main_container.GetId():  # main_container
            self.Layout()

        elif ev_id == self.image_panel_container.GetId():   # image container
            self.change_image_scale()

        elif ev_id == self.prop_mask_mode_val.GetId():  # transparency mode
            if self.prop_mask_mode_val.GetCurrentSelection() == 1:
                self.prop_transp_col_lable.Enable(True)
                self.prop_transp_col_val.Enable(True)
            elif self.prop_mask_mode_val.GetCurrentSelection() == 0:
                self.prop_transp_col_lable.Enable(False)
                self.prop_transp_col_val.Enable(False)

        elif ev_id == self.prop_transp_col_val.GetId():     # set transp color

            with wx.ColourDialog(self, self.col_data) as col_dlg:
                if col_dlg.ShowModal() == wx.ID_CANCEL:
                    return
                self.transp_colour = wx.Colour(
                    col_dlg.GetColourData().GetColour()
                )

                self.prop_transp_col_val.SetLabel(
                    self.transp_colour.GetAsString(wx.C2S_HTML_SYNTAX)
                )
                self.prop_transp_col_val.SetBackgroundColour(
                    self.transp_colour
                )

                self.col_data.SetColour(self.transp_colour)

        elif ev_id == self.prop_image_open.GetId():     # open image file
            self.open_file_flag = True

            wildcard = ('Images file (*.bmp *.gif *.png *.jpg)|'
                        '*.bmp;*.gif;*.png;*.jpg|'
                        'All files|*.*')
            with wx.FileDialog(
                    self,
                    'Open image files',
                    wildcard=wildcard,
                    defaultDir=self.files_directory,
                    style=wx.FD_OPEN | wx.FD_MULTIPLE | wx.FD_CHANGE_DIR
            ) as open_file_dlg:
                if open_file_dlg.ShowModal() == wx.ID_CANCEL:
                    return

                self.files_directory = open_file_dlg.GetDirectory()
                self.files_list = dict.fromkeys(open_file_dlg.GetFilenames())
                to_copy = dict(path=self.files_directory, palette=None,
                               image=None)
                for key in self.files_list:
                    self.files_list[key] = to_copy.copy()

            self.exit_list_generate()

            self.change_image_scale()

        elif ev_id == self.prop_exit_btn.GetId():   # save file(s)
            if len(self.files_list) == 1:   # single file
                wildcard = 'STL file (*.stl)|*.stl|All files|*.*'
                with wx.FileDialog(
                        self, 'Save File(s)',
                        wildcard=wildcard,
                        defaultDir=self.exit_file_dir,
                        defaultFile=self.exit_files[0],
                        style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
                ) as save_file_dlg:
                    if save_file_dlg.ShowModal() == wx.ID_CANCEL:
                        return
                    pattern = r'^(.+)\.\w{1,4}$'
                    file = save_file_dlg.GetFilename()
                    if not re.match(pattern, file):
                        file = f'{file}.stl'
                    self.exit_files[0] = file
                    self.exit_file_dir = save_file_dlg.GetDirectory()

                    label = os.path.join(self.exit_file_dir,
                                         self.exit_files[0])
                    self.prop_exit_label.SetLabel(label)
                    self.prop_exit_label.SetToolTip(label)

            elif len(self.files_list) > 1:  # multi files
                with wx.DirDialog(
                        self,
                        'Save to directory',
                        defaultPath=self.exit_file_dir
                ) as save_dir_dlg:
                    if save_dir_dlg.ShowModal() == wx.ID_CANCEL:
                        return
                    self.exit_file_dir = save_dir_dlg.GetPath()

                    self.prop_exit_label.SetLabel(save_dir_dlg.GetPath())
                    self.prop_exit_label.SetToolTip(save_dir_dlg.GetPath())

        elif ev_id == self.execute_button.GetId():  # execute process
            mutex = Lock()

            kwargs = dict(
                density=self.prop_den_val.GetValue(),
                max_h=self.prop_max_height_val.GetValue(),
                min_h=self.prop_min_height_val.GetValue(),
                exit_type=self.prop_exit_format.GetStringSelection(),
                mode='Pixel',
                h_mode=self.prop_height_mode_val.GetStringSelection(),
                mask_mode=self.prop_mask_mode_val.GetStringSelection(),
                color=self.transp_colour.Get(False),
                image_path=self.files_directory,
                image_files=self.files_list,
                save_path=self.exit_file_dir,
                save_files=self.exit_files,
                auto_crop=self.prop_image_autocrop.GetValue()
            )
            self.converter = ConverterThread(self, kwargs)
            self.updater = UpdaterThread(self, mutex)

            self.progress_bar.SetValue(0)
            event.GetEventObject().Disable()

            self.converter.start()
            self.updater.start()

        elif ev_id == self.note.GetId():
            if event.GetEventType() == nb_page_changed:     # PAGE CHANGED
                self.palette_updater(self.note.GetSelection())

            elif event.GetEventType() == nb_page_closed:    # PAGE CLOSED
                temp = {self.note.GetPageText(idx)
                        for idx in range(self.note.GetPageCount())}

                to_delete = temp ^ set(self.files_list.keys())
                for key in to_delete:
                    self.files_list.__delitem__(key)

                self.exit_list_generate()

    def exit_list_generate(self):
        pattern = r'^(.+)\.\w{1,4}$'
        self.exit_files.clear()
        for i in self.files_list:
            self.exit_files.append(f'{re.search(pattern, i).group(1)}.stl')

        if len(self.files_list) == 1:   # single file
            label = os.path.join(self.exit_file_dir, self.exit_files[0])
            self.prop_exit_label.SetLabel(label)
            self.prop_exit_label.SetToolTip(label)

        elif len(self.files_list) > 1:  # multi files
            self.prop_exit_label.SetLabel(self.exit_file_dir)
            self.prop_exit_label.SetToolTip(self.exit_file_dir)

        if self.open_file_flag:
            self.execute_button.Enable()
            self.prop_exit_btn.Enable()

    def change_image_scale(self):
        if self.files_list is None or len(self.files_list) == 0:
            return

        cont_x, cont_y = self.image_container.GetSize()

        self.note.DeleteAllPages()

        for file in self.files_list:
            path = os.path.join(self.files_list[file]['path'], file)

            ##########################
            ##########################
            with Image.open(path) as img:
                if self.prop_image_autocrop:
                    img = img.crop(img.getbbox())

            if img.mode != 'LA':
                img = img.convert('P', palette=Image.ADAPTIVE)
            img = img.convert(mode='RGBA')

            self.files_list[file]['palette'] = img.getcolors()
            self.files_list[file]['image'] = img

            ##########################
            ##########################

            image = wx.Image(
                size=img.size,
                data=img.convert(mode='RGB').tobytes(),
                alpha=img.tobytes()[3::4]
            )

            image_w, image_h = image.GetSize()

            image_ratio = image_w / image_h
            cont_ratio = cont_x / cont_y

            if cont_ratio > image_ratio:
                w = (image_w * cont_y) / image_h * 0.75
                h = cont_y * 0.75
            else:
                w = cont_x * 0.75
                h = (image_h * cont_x) / image_w * 0.75

            image = image.Rescale(w, h)

            window = wx.ScrolledWindow(self.note, 0,
                                       style=wx.HSCROLL | wx.VSCROLL)

            image = wx.StaticBitmap(
                window,
                wx.ID_ANY,
                image.ConvertToBitmap()
            )

            sizer = wx.BoxSizer(wx.VERTICAL)
            sizer.Add(image, 1, wx.ALL | wx.EXPAND)

            window.SetSizer(sizer)
            window.Layout()
            sizer.Fit(window)
            self.note.AddPage(window, file, False)

        self.image_container.SetSizer(self.image_sizer)
        self.image_container.Layout()
        self.image_panel_container.Layout()

    def palette_updater(self, select):
        self.palette_grid.Clear()

        self.palette_grid.Append(pg.PropertyCategory('amount | color', 'cat'))

        try:
            palette = self.files_list[self.note.GetPageText(select)]['palette']
        except KeyError:
            return
        if palette is None:
            return
        k = 0
        for amount, colour in sorted(palette,
                                     key=lambda x: rgb_to_hsv(*x[1][:3]),
                                     reverse=False):
            if colour[3] > 0:
                self.palette_grid.Append(pg.ColourProperty(
                    label=str(amount),
                    name=str(k),
                    value=wx.Colour(colour)
                ))
                k += 1

        self.palette_grid.SetSplitterPosition(70)

    def status_update(self, msg):
        """
        Retrieves data from the stream and updates the progress bar
        """

        key, value = msg.data
        if key == 'init':
            self.progress_bar.SetRange(value)
            self.progress_bar.SetValue(0)

        elif key == 'end':
            self.execute_button.Enable()
            self.progress_bar.SetValue(self.progress_bar.GetRange())
            self.progress_label.SetLabel('Done!')
            self.prop_execute_sizer.Layout()

        elif key == 'name':
            self.progress_label.SetLabel(value)
            self.prop_execute_sizer.Layout()

        elif key == 'count':
            self.progress_bar.SetValue(value)


class MyApp(wx.App):
    def OnInit(self):
        self.frame = MyFrame()
        self.frame.Show()
        self.SetTopWindow(self.frame)
        return True


if __name__ == '__main__':
    app = MyApp()
    app.MainLoop()
