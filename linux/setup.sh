#!/bin/bash

echo Creating Virtual Environment...
echo
python3 -m venv venv
source venv/bin/activate
python3 -m pip install --upgrade pip
pip install --upgrade setuptools
echo
echo Installing Dependency...
echo
grep -v wxPython requirements.txt >> tmp.txt
pip install -r tmp.txt
rm tmp.txt
echo
echo Done!
echo
deactivate
echo ATTENTION!!!
echo Please install wxPython in manually.
echo For this follow the link: https://wxpython.org/pages/downloads/
