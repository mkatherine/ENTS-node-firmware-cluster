from pymeasure.instruments.keithley import Keithley2450

smu = Keithley2450("TCPIP::128.114.204.83::5025::SOCKET")
smu.status()
