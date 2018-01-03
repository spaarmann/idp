
#Achtung: Bisher nur Prototypen: Funktionen zum Auslesen der Sensoren müssen implementiert werden!

class Sensor:
	def __init__(self, pinnumber):
		self._pinnumber = pinnumber
		self._lastValue = 0
	def _read(self):#Sollte in den Abgeleiten Klassen implementiert werden
		#print("_read(Sensor)")
		pass
	def _update(self):
		pass
	def readValue(self):
		#print("readValue(Sensor): ")
		self._update()
		return self._lastValue
		pass


class Temperatursensor(Sensor):
	def __init__(self, pinnumber):
		super().__init__(pinnumber)
	
	def _read(self):
		#print("[TS] Wert einlesen")
		return self._lastValue + 1
	
	#lastValue Sensorspezifisch aktualisieren
	def _update(self):
		#print("[TS] Update Value")
		#Kommunikation mit TemperaturSensor (Wert anfordern)
		#self._write("UPDATE TS")
		#Wert einlesen
		self._lastValue = self._read()
	
	def _write(self, text):		
		#print("[TS]Schreibe(Pin: ", self._pinnumber, "): ", text)
		pass


class PHsensor(Sensor):
	def __init__(self, pinnumber):
		super().__init__(pinnumber)
	
	def _read(self):
		#print("[PH] Wert einlesen ", self._lastValue)
		return self._lastValue + 1
	
	#lastValue Sensorspezifisch aktualisieren
	def _update(self):
		#print("[PH] Update Value")
		#Kommunikation mit TemperaturSensor (Wert anfordern)
		self._write("UPDATE TS")
		#Wert einlesen
		self._lastValue = self._read()
	
	def _write(self, text):
		#print("[PH]Schreibe(Pin: ", self._pinnumber, "): ", text)
		pass