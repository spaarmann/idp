import RPi.GPIO as GPIO
from Sensor import *
import datetime
import time

GPIO.setmode(GPIO.BCM)

#PIN und Sensorkonfigurationen
PINCO2 = 17 #CO2
PINGAS = 27 #Druckluft
PINFOOD = 22 #Dünger

#(
PINTEMP = 23
PINPH = 24

#Sensorinitalisierung
tsens = Temperatursensor(PINTEMP)
phsens = PHsensor(PINPH)
#)

#Zustände
Co2 = False #CO2 Magnetventil öffnen?
Gas = False #Druckluft Magnetventil öffnen
Food= False # Dünger?
temp = 0 # Aktuelle Temperatur
ph = 0 # Aktueller PH-Wert

#Konfigurationsmöglichkeiten (+ Siehe calculateState)-----------------------------------

#Test für Zeitabhängigkeit:
#Start und Endzeit(--,--) <= (Stunde, Minute)
st = datetime.time(8,00).strftime("%H:%M:%S")
et = datetime.time(20,00).strftime("%H:%M:%S")

sleepTime = 10 # Pause zwischen den Aktualisiervorgängen #in Sekunden

#Hilfs- und Zählvariablen


#---------------------------------------------------------------------------------------


#Sensorwerte einlesen und abspeichern
def readSensors():
    global temp
    global ph
    temp = tsens.readValue()
    ph = phsens.readValue()
    return


def switchState(state):
    if state:
        return False
    else:
        return True

#Gewünschter Zustand der Geräte in Abhängigkeit der Sensoren, Uhrzeit, Formeln... festlegen
def calculateState():
    global Co2
    global Gas
    global Food
    global lastStateChange
    #----------------------

    now = datetime.datetime.now()
    n = now.strftime("%H:%M")


    if isInTimePeriod(st, et, n):
        print("Innerhalb Zeitspanne")
        Co2 = True
        Gas = True
        Food = True
    else:
        print("Außerhalb Zeitspanne")
        Co2 = False
        Gas = False
        Food = False

    #----------------------
    return

#Zustände ausführen, Geräte/Pins schalten
def performState():
    GPIO.output(PINCO2, Co2)
    GPIO.output(PINFOOD, Food)
    GPIO.output(PINGAS, Gas)
    pass

#Kleine Hilfsmethode, Zur Ausgabe des Zustandes
def getSwitchString(state):
    if state:
        return "AN"
    else:
        return "AUS"

def isInTimePeriod(startTime, endTime, nowTime):
    if startTime < endTime:
        return nowTime >= startTime and nowTime <= endTime
    else: #Over midnight
        return nowTime >= startTime or nowTime <= endTime

def initPins():
    GPIO.setup(PINCO2, GPIO.OUT)
    GPIO.setup(PINGAS, GPIO.OUT)
    GPIO.setup(PINFOOD, GPIO.OUT)

#Daten loggen + an Server senden/Kommunikation ausführen...
def logState():
    localtime = time.asctime( time.localtime(time.time()) )
    print(localtime)
    print("Temperatur: ", temp)
    print("PH: ", ph)
    print("CO2: ", getSwitchString(Co2))
    print("Druckluft: ", getSwitchString(Gas))
    print("Dünger: ", getSwitchString(Food))



if __name__ == "__main__":
    initPins();
    while True:
        readSensors()
        calculateState()
        performState()
        logState()
        time.sleep(sleepTime)
