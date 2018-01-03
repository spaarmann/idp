#import RPi.GPIO as GPIO
from Sensor import *
import time


#PIN und Sensorkonfigurationen
PINCO2 = 1 #CO2
PINGAS = 2 #Druckluft
PINFOOD = 3 #Dünger
PINTEMP = 4
PINPH = 5

#Sensorinitalisierung
tsens = Temperatursensor(PINTEMP)
phsens = PHsensor(PINPH)

#Zustände
Co2 = False #CO2 Magnetventil öffnen?
Gas = False #Druckluft Magnetventil öffnen
Food= False # Dünger?
temp = 0 # Aktuelle Temperatur
ph = 0 # Aktueller PH-Wert

#Konfigurationsmöglichkeiten (+ Siehe calculateState)
changeStateSeconds = 10

sleepTime = 5 # Pause zwischen den Aktualisiervorgängen

#Hilfs- und Zählvariablen 
lastStateChange = 0 # Sekunden



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

  

   

    print("\n\nLetzte : ", lastStateChange, " Sek.")

    if lastStateChange >= changeStateSeconds:
        lastStateChange = 0
        Co2 = switchState(Co2)
        Gas = switchState(Gas)
        Food = switchState(Food)
        
    lastStateChange += sleepTime
    return

#Zustände ausführen, Geräte/Pins schalten
def performState():
    #GPIO.output(PINCO2, co2)
    pass

#Kleine Hilfsmethode, Zur Ausgabe des Zustandes       
def getSwitchString(state):
    if state:
        return "AN"
    else:
        return "AUS"

def initPins():
    #GPIO.setmode...
    pass

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
    while True:
        readSensors()
        calculateState()
        performState()
        logState()
        time.sleep(sleepTime)
