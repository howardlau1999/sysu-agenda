from distutils.core import setup, Extension

pyagenda = Extension('pyagenda',
                     include_dirs=['./include'],
                     language="c++",
                     extra_compile_args=['-std=c++11'],
                     sources=['src/PyAgenda.cpp', 'src/User.cpp', 'src/Storage.cpp', 'src/Meeting.cpp', 'src/Date.cpp', 'src/AgendaService.cpp'])

setup(name='PyAgenda',
      version='0.3',
      description='Python interfaces of Agenda',
      ext_modules=[pyagenda])
