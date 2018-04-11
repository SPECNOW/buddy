import serial
import time
import pytest
import os
import sys
import subprocess

delfinoPort = os.environ.get('DELFINO_COM_PORT', 'COM10')
arduinoPort = os.environ.get('ARDUINO_COM_PORT', 'COM9')

# TODO: Need to somehow get Arduino and Delfino to automatically 
#        flash correct test programs before running this

class Arduino:
    def __init__(self,  arduino_ino, arduino_path='C:\\Program Files (x86)\\Arduino', arduino_port=arduinoPort):
        if arduino_ino:
            print('Compiling {}'.format(arduino_ino))
            
            curDir = os.path.abspath(os.curdir)
            os.chdir(arduino_path)
            proc = subprocess.Popen(
                [
                    "arduino_debug.exe",
                    "--upload",
                    "--board", "arduino:avr:mega",
                    "--port", arduino_port,
                    arduino_ino
                ],
                stdout = subprocess.PIPE,
                stderr = subprocess.PIPE
            )
            while True:
                line = proc.stdout.readline()
                print(line.rstrip('\n'))
                if not line:
                    break
            proc.wait()
            print("ERR:\n\t{}\n".format('\t'.join(proc.stderr.readlines())))
            os.chdir(curDir)
            
            if (proc.returncode):
                print("Failed to compile")
                sys.exit()

            print('Compile and upload success!')
        else:
            print("Arduino INO not provided")
        return

class Delfino:
    def __init__(self, project='Buddy', workspace=os.path.abspath('..'), ccs_path=r"C:\ti\ccsv7\eclipse", script=os.path.abspath(r'tests\upload_delfino.js')):
        # Only compile and upload the first time
        if not os.environ.get('_DELFINO_COMPILED', False):
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
                '-application', 'com.ti.ccstudio.apps.projectBuild',
                '-ccs.projects', project
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        while True:
            line = proc.stdout.readline()
            print(line.rstrip('\n'))
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
            stderr=subprocess.PIPE
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
        cls.serialDelfino.baurdrate = 115200
        cls.serialDelfino.port = delfinoPort
        cls.serialDelfino.timeout = 1
        cls.serialDelfino.open()
        # Clear the Buffer
        cls.serialDelfino.read()        
        print("Delfino Serial Initialized")

        print("Compiling and uploading Arduino")
        cls.arduino = Arduino(arduino_ino=cls.arduino_ino)
        
        cls.serialArduino = serial.Serial()
        cls.serialArduino.baurdrate = 115200
        cls.serialArduino.port = arduinoPort
        cls.serialArduino.timeout = 1
        cls.serialArduino.open()
        # Clear the Buffer
        cls.serialArduino.read()
        print("Arduino Serial Initialized")
        
        return

    def teardown_class(cls):
        if cls.serialArduino:
            cls.serialArduino.close()
        if cls.serialDelfino:
            cls.serialDelfino.close()
        return