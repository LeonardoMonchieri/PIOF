# PIOF
Un progettto che mediante l'utilizzo di due ESP-8266 permette la gestione intelligente di un orto pensile attraverso l'osservazione delle condizioni meteorologiche e l'integrazione di una web-app.

#Componenti utilizzate
##Stazione meteo esterna
- ESP 8266-12F 
- Sensore di temperatura e umiditá DH11
- Sensore di pioggia/neve
- Fotoresistore 
- 2 Diodo 1N4007

##Controller interno
- ESP 8266-12F
- Schermo i2c Oled 128"x64"
- Led Blu
- Led Verde
- Encoder rotativo 
- 2 Resistenze 10K 
- 2 Resistenze 

#Obbiettivo
L'obbiettivo del progetto é quello di creare un sistema di gestione intelligente dell'acqua per l'irraigazione di un orto pensile attraverso le rilevazione fatte da una stazione meteo posta in corrispondenza della zona di interesse e delle impostazioni definite in locale, su un apposito controller, oppure online attraverso una web-app installata proprio sul controller.

#Funzionamento
Il funzionamento del progetto si sviluppa du 3 piani differenti che vengono integrati attraverso la scheda _controller_:
1.**Controller locale**
2.**Stazione meteo esterna**
2.**Web-app**
