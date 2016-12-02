from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

# fetch values from package.xml
setup_args = generate_distutils_setup(
    packages=['motor_control_drivers'],
    package_dir={'src': 'src', 'msg': 'msg'},
)

setup(**setup_args)