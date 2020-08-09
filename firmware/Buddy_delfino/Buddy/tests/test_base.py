import serial
import time
import pytest
import os
import sys
import subprocess

delfinoPort = os.environ.get('DELFINO_COM_PORT', 'COM4')
arduinoPort = os.environ.get('ARDUINO_COM_PORT', 'COM3')
baudrate = 115200

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

    def compile(self, arduino_path='D:\\Program Files (x86)\\Arduino'):
        print('Compiling {}'.format(self.arduinoIno))
        curDir = os.path.abspath(os.curdir)
        os.chdir(arduino_path)
        proc = subprocess.Popen(
            [
                "arduino_debug.exe",
                "--upload",
                "--board", "arduino:avr:mega",
                "--port", self.arduinoPort,
                self.arduinoIno
            ],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE,
            encoding='UTF-8'
        )
        #import ipdb; ipdb.set_trace()
        while True:
            line = proc.stdout.readline()
            #print(line.rstrip('\n'))
            if not line:
                break
        proc.wait()
        print("ERR:\n\t{}\n".format('\t'.join(proc.stderr.readlines())))
        os.chdir(curDir)
        
        if (proc.returncode):
            print("Failed to compile")
            sys.exit()

        print('Compile and upload success!')

class Delfino:
    def __init__(self, project='Buddy', workspace=os.path.abspath('..'), ccs_path=r"D:\ti\ccs1010\ccs\eclipse", script=os.path.abspath(r'tests\upload_delfino.js')):
        # Only compile and upload the first time
        if not os.environ.get('_DELFINO_COMPILED', False):
            os.system("Taskkill /IM eclipsec.exe /F")
            self.compile(
                project=project,
                workspace=workspace,
                ccs_path=ccs_path
            )
            self.upload(
                script=script,
                workspace=workspace,
                ccs_path=ccs_path
            )
            print('Compile and upload success!')
            os.environ['_DELFINO_COMPILED'] = '1'
        else:
            print("Delfino already flashed")
        return
    
    def compile(self, project, workspace, ccs_path):
        print('Compiling {}'.format(project))
        
        curDir = os.path.abspath(os.curdir)
        os.chdir(ccs_path)
        proc = subprocess.Popen(
            [
                'eclipsec.exe',
                '-noSplash',
                '-data', workspace,
                '-application', 'com.ti.ccstudio.apps.buildProject',
                '-ccs.projects', project
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            encoding='UTF-8'
        )
        while True:
            line = proc.stdout.readline()
            #print(line.rstrip('\n'))
            if not line:
                break
        proc.wait()
        print("ERR:\n\t{}\n".format('\t'.join(proc.stderr.readlines())))
        os.chdir(curDir)

        if (proc.returncode):
            print("Failed to compile")
            sys.exit()
        return

    def upload(self, workspace, script, ccs_path):
        print("Uploading compiled project to Delfino")
        curDir = os.path.abspath(os.curdir)
        os.chdir(ccs_path)
        proc = subprocess.Popen(
            [
                'eclipsec.exe',
                '-noSplash',
                '-data', workspace,
                '-application', 'com.ti.ccstudio.apps.runScript',
                '-ccs.script', script
            ],
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            encoding='UTF-8'
        )
        success = False
        while True:
            line = proc.stdout.readline()
            print(line.rstrip('\n'))
            # Script should print 'Running!' when it is running
            if "Running!" in line:
                # Target is running, kill script since we're done with it
                success = True
                break
            if not line:
                break
        os.chdir(curDir)
        
        if (not success):
            print("Failed to compile")
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