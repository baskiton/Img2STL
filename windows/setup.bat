@echo off

echo Creating Virtual Environment...
echo.
call python -m venv %venv

call %venv\scripts\activate.bat

call python -m pip install --upgrade pip
call pip install --upgrade setuptools

echo.
echo Installing Dependency...
echo.
call pip install -r requirements.txt

call %venv\scripts\deactivate.bat

echo.
echo Done!
pause
exit