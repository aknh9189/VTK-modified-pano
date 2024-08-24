from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os, sys
from pathlib import Path
import logging

logger = logging.getLogger(__name__)

class CMakeBuild(build_ext):
    def run(self):
        self.cwd = Path(os.getcwd())
        assert self.cwd.name == 'VTK-modified-pano'
        if not self.dry_run:
            print("Mission accomplished!")
            print(os.getcwd())
            self.run_cmake()
        # Assuming CMakeLists.txt is in the same directory as setup.py
        #build_temp = self.build_temp
        #os.makedirs(build_temp, exist_ok=True)

        ## Configure CMake project
        #self.spawn(['cmake', '.', f'-B{build_temp}'])

        ## Build the project
        #self.spawn(['cmake', '--build', build_temp])

        ## Assuming CMake install command copies the necessary files to the right place
        super().run()

    def run_cmake(self):
        import importlib.util
        if importlib.util.find_spec('cmake') is not None:
            print("WARNING: if you have a pip installed copy of cmake installed and are getting 'CMake not found errors', try uninstalling it")

        build_dir = self.cwd / 'build'
        build_dir.mkdir(exist_ok=True) 

        cmake_command = [
            'cmake',
            '-DVTK_OPENGL_HAS_EGL:BOOL=ON',
            '-DVTK_USE_X:BOOL=OFF',
            '-DVTK_BUILD_SHARED_LIBS:BOOL=OFF',
            '-DBUILD_TESTING=OFF',
            '-DCMAKE_BUILD_TYPE=Release',
            '-DVTK_WRAP_PYTHON=ON',
            '-DVTK_WHEEL_BUILD=ON',
            '-DVTK_PYTHON_VERSION=3',
            '-DVTK_ENABLE_LOGGING=OFF',
            '-DVTK_USE_MPI=ON',
            '-DVTK_USE_CUDA=ON',
            f'-B{build_dir}',
            f'-S{self.cwd}'
        ]
        # cmake
        print("running cmake")
        self.spawn(cmake_command)
        # build
        print("building package")
        self.spawn(['cmake', '--build', str(build_dir), '-j'])
        # install the wheel
        print("INSTALLING package")
        os.chdir('build')
        self.spawn([sys.executable, 'setup.py', 'install'])


setup(
    name='VTK-modified-pano',
    version='0.1.0',
    packages=[],
    ext_modules=[Extension('dummy', sources=[])],  # This can be necessary for the build_ext command to run
    cmdclass={
        'build_ext': CMakeBuild,
    },
)

