@echo off
call %venv\scripts\activate.bat
start pythonw main.py
call %venv\scripts\deactivate.bat
exit