import os
import wx

from datetime import datetime


class Logging(wx.Log):
    def __init__(self, log_file_name: str) -> None:
        super().__init__()
        self._old_logger: wx.Log = wx.Log.GetActiveTarget()
        self._log_file_name = os.path.join(os.getcwd(),  log_file_name)

        wx.Log.SetActiveTarget(self)

        with open(self._log_file_name, "w+") as flog:
            flog.write(f"Session started at {wx.DateTime.Now().FormatISODate()}\n")

    def __del__(self) -> None:
        wx.Log.SetActiveTarget(self._old_logger)

    def do_log_line(self, level: str, timestr: str, msg: str) -> None:
        with open(self._log_file_name, "a+") as flog:
            t_str = f"<{level:8}> {timestr:8} {msg}\n"
            flog.write(t_str)

    def DoLogRecord(self, level, msg: str, info: wx.LogRecordInfo) -> None:
        postfix = ""

        if level <= wx.LOG_Warning or level == wx.LOG_Trace or level == wx.LOG_Debug:
            postfix = f"[{info.func}() in {info.filename}({info.line})]"
            self._old_logger.LogRecord(level, f"{level}: {msg}", info)

        if level == wx.LOG_Error:
            level_str = "ERROR"
        elif level == wx.LOG_Warning:
            level_str = "WARNING"
        elif level == wx.LOG_Message:
            level_str = "MESSAGE"
        elif level == wx.LOG_Status:
            level_str = "STATUS"
        elif level == wx.LOG_Info:
            level_str = "INFO"
        elif level == wx.LOG_Debug: 
            level_str = "DEBUG"
        elif level == wx.LOG_Trace:
            level_str = "TRACE"
        elif level == wx.LOG_Progress:
            level_str = "PROGRESS"
        else:
            level_str = "USER"

        self.do_log_line(
            level_str,
            datetime.utcfromtimestamp(info.timestamp).strftime("%H:%M:%S"),
            f"{msg} {postfix}"
        )
