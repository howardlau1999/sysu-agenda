from distutils.core import setup, Extension

pyagenda = Extension('pyagenda',
                     include_dirs=['./include'],
                     language="c++",
                     sources=['src/PyAgenda.cpp', 'src/User.cpp', 'src/Storage.cpp', 'src/Meeting.cpp', 'src/Date.cpp', 'src/AgendaService.cpp'])

setup(name='PyAgenda',
      version='0.2',
      description='Python interfaces of Agenda',
      ext_modules=[pyagenda])
