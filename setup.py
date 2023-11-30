from setuptools import setup, Extension, find_packages


_chempath = Extension(
    'chempath.core._chempath',
    sources = [
        'chempath/core/chempath.c',
        'chempath/core/src/chemtypes.c',
        'chempath/core/src/step.c',
        'chempath/core/src/substance.c',
        'chempath/core/src/element.c',
        'chempath/core/src/common.c',
        'chempath/core/src/hashtable.c',        
    ],
    include_dirs = ['chempath/core/include'],
)


setup(
    name         = 'chempath',
    version      = '0.0.1',
    packages     = find_packages(),
    ext_modules  = [_chempath],
    description  = 'Chemical reaction path.',
    author       = 'Zhao Kunwang',
    author_email = 'clumsykundev@gmail.com',
    url          = 'https://github.com/clumsykun/chempath',
)
