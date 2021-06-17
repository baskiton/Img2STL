import pathlib
import sys

from setuptools import setup, find_packages

# Check python version
MINIMAL_PY_VERSION = (3, 7)
if sys.version_info < MINIMAL_PY_VERSION:
    raise RuntimeError('This app works only with Python {}+'.format('.'.join(map(str, MINIMAL_PY_VERSION))))


def get_version():
    here = pathlib.Path(__file__).parent
    file =(here / 'Img2STL' / '__init__.py').read_text('utf-8')
    for line in file.splitlines():
        if line.startswith('__version__'):
            delim = '"' if '"' in line else "'"
            return line.split(delim)[1]


def get_long_description():
    with open("README.md", "r", encoding="utf-8") as readme:
        return readme.read()


setup(
    name='Img2STL',
    version=get_version(),
    url='https://github.com/baskiton/Img2STL',
    project_urls={
        "Bug Tracker": "https://github.com/baskiton/Img2STL/issues",
    },
    license='MIT',
    author='Alexander Baskikh',
    python_requires='>=3.7',
    author_email='baskiton@gmail.com',
    description='Application for converting images to STL (3D model for printing)',
    long_description=get_long_description(),
    long_description_content_type="text/markdown",
    packages=find_packages(),
    install_requires=[
        'numpy==1.20.3',
        'Pillow==8.2.0',
        'six==1.16.0',
        'wxPython==4.1.1;platform_system!="Linux"',
    ],
    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Development Status :: 4 - Beta',
        'Environment :: Console',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Topic :: Multimedia :: Graphics :: 3D Modeling',
    ]
)
