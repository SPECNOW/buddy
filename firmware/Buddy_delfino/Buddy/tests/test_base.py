import serial
import time
import pytest
import os
import sys
import subprocess

delfinoPort = os.environ.get('DELFINO_COM_PORT', 'COM4' if sys.platform != 'linux' else '/dev/ttyUSB0')
arduinoPort = os.environ.get('ARDUINO_COM_PORT', 'COM3' if sys.platform != 'linux' else '/dev/ttyACM0')
baudrate = 115200

CCS_PATH = os.path.join("D:", "ti", "ccs1010", "ccs") if sys.platform != 'linux' else \
    os.path.join('/', 'opt', 'ti', 'ccs1011', 'ccs')
UNIFLASH_PATH = os.path.join("D:", "ti", "uniflash_6.1.0")if sys.platform != "linux" else \
    os.path.join("/", "opt", "ti", "uniflash_5.0.0")

# TODO: Need to somehow get Arduino and Delfino to automatically 
#        flash correct test programs before running this

class Arduino:
    def __init__(self,  arduino_ino, arduino_port=arduinoPort):
        if arduino_ino:
            self.arduinoPort = arduinoPort
            self.arduinoIno = arduino_ino
            self.compile()
        else:
            print("Arduino INO not provided")
        return

    def compile(self):
        print('Compiling {}'.format(self.arduinoIno))
        curDir = os.path.abspath(os.curdir)
        proc = subprocess.run(
            [
                "D:\\arduino-cli\\arduino-cli.exe" if sys.platform != 'linux' else "arduino-cli",
                'compile',
                "--upload",
                '--fqbn', "arduino:avr:mega",
                "--port", self.arduinoPort,
                self.arduinoIno
            ],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE,
            encoding='UTF-8'
        )
        print("RET:\n{out}\nERR:\n{err}\n".format(
            out='\t'.join([line + '\n' for line in proc.stdout.split('\n')]),
            err='\t'.join([line + '\n' for line in proc.stderr.split('\n')])
        ))
        
        if (proc.returncode):
            print("Failed to compile")
            sys.exit()

        print('Compile and upload success!')

class Delfino:
    def __init__(self, project='Buddy', workspace=os.path.abspath('..')):
        # Only compile and upload the first time
        if not os.environ.get('_DELFINO_COMPILED', False):
            self.compile(
                project=project,
                workspace=workspace,
                ccs_path=CCS_PATH
            )
            self.upload(
                project=project,
                workspace=workspace,
                uniflash_dir=UNIFLASH_PATH
            )
            print('Compile and upload success!')
            os.environ['_DELFINO_COMPILED'] = '1'
        else:
            print("Delfino already flashed")
        return
    
    def compile(self, project, workspace, ccs_path):
        print('Compiling {}'.format(project))
        
        clean = subprocess.Popen(
            [   
                os.path.join(ccs_path, 'utils', 'bin', 'gmake'),
                '-k',
                '-j', '4',
                'clean',
                '-O'
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=os.path.join(workspace, project, 'CPU1_RAM'),
            encoding='UTF-8'
        )
        while True:
            line = clean.stdout.readline()
            print(line.rstrip('\n'))
            if not line:
                break
        clean.wait()
        print("ERR:\n\t{}\n".format('\t'.join(clean.stderr.readlines())))
        print(clean.stdout.readlines())
        proc = subprocess.Popen(
            [   
                os.path.join(ccs_path, 'utils', 'bin', 'gmake'),
                '-k',
                '-j', '4',
                'all',
                '-O'
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=os.path.join(workspace, project, 'CPU1_RAM'),
            encoding='UTF-8'
        )
        while True:
            line = proc.stdout.readline()
            print(line.rstrip('\n'))
            if not line:
                break
        proc.wait()
        print("ERR:\n\t{}\n".format('\t'.join(proc.stderr.readlines())))

        if (proc.returncode):
            print("Failed to compile")
            sys.exit()
        return

    def upload(self, workspace, project="Buddy", uniflash_dir="/opt/ti/uniflash_5.0.0"):
        print("Uploading compiled project to Delfino")
        ret = os.system(" ".join([
            os.path.join(uniflash_dir, "deskdb", "content", "TICloudAgent", 'win' if sys.platform != 'linux' else 'linux', "ccs_base", "DebugServer", "bin", "DSLite.exe" if sys.platform != "linux" else "DSLite"),
            "flash",
            "-c", os.path.join(workspace, project, "targetConfigs", "TMS320F28377S.ccxml"),
            "-l", "generated.ufsettings",
            "-s", 'VerifyAfterProgramLoad="No verification"',
            "-e", "-f", "-v", os.path.join(workspace, project, "CPU1_RAM", "Buddy.out")
        ]))
        if ret:
            sys.exit()
        return

class TestBase(object):
    arduino_ino = None
    delfino_project = None
    arduino = None
    delfino = None

    def setup_class(cls):
        print("Compiling and uploading Delfino")
        cls.delfino = Delfino(project=cls.delfino_project)

        cls.serialDelfino = serial.Serial()
        cls.serialDelfino.baudrate = baudrate
        cls.serialDelfino.port = delfinoPort
        cls.serialDelfino.timeout = 1
        cls.serialDelfino.open()
        # Clear the Buffer
        cls.serialDelfino.read()        
        print("Delfino Serial Initialized")
      
        if cls.arduino_ino:
            print("Compiling and uploading Arduino")
            cls.arduino = Arduino(arduino_ino=cls.arduino_ino)
            cls.serialArduino = serial.Serial()
            cls.serialArduino.baudrate = baudrate
            cls.serialArduino.port = arduinoPort
            cls.serialArduino.timeout = 1
            cls.serialArduino.open()
            # Clear the Buffer
            cls.serialArduino.read()
            print("Arduino Serial Initialized")
        else:
            print("Arduino will not be intialized")
            cls.arduino = None
            cls.serialArduino = None        
        return

    def teardown_class(cls):
        if cls.serialArduino:
            cls.serialArduino.close()
        if cls.serialDelfino:
            cls.serialDelfino.close()
        return