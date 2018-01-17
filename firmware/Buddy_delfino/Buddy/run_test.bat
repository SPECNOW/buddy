python -m pip install -r requirements.txt
python -m pytest -x -s tests\test_serial.py -k TestDelfinoSerialRx
python -m pytest -x -s tests\test_serial.py -k TestDelfinoSerialTx