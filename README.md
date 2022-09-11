# PIOF
Un progettto che mediante l'utilizzo di due ESP-8266 permette la gestione intelligente di un orto pensile attraverso l'osservazione delle condizioni meteorologiche e l'integrazione di una web-app.

# Componenti utilizzate
## Stazione meteo esterna
- ESP 8266-12F 
- Sensore di temperatura e umiditá DH11
- Sensore di rilevamento pioggia/neve
- Fotoresistore 
- Regolatore di voltaggio L7805CV3
- Condensatore elettrolitico 33µF 160V
- Condensatore in ceramica 100nF
- Batteria da 9V
- 2 Diodo 1N4007

## Controller interno
- ESP 8266-12F
- Schermo i2c Oled 128"x64"
- Relé 5V
- Pulsante
- Led Verde
- Encoder rotativo 
- Resistenze 220Ω
- 2 Resistenze 10KΩ
- Elettrovalvola 12V da 1/2"

# Librerie utilizzate

## Connessione internet e servizi     
- ESP8266WiFi
- WiFiUdp
- ESPAsyncTCP
- ESPAsyncWebServer
## Sesnsori e util
- DHT  
- TimeLib
- List
## Utilizzo schermo Oled
- Wire
- Adafruit_GFX
- Adafruit_SSD1306



# Obbiettivo
L'obbiettivo del progetto é quello di creare un sistema di gestione intelligente dell'acqua per l'irraigazione di un orto pensile attraverso le rilevazione fatte da una stazione meteo posta in corrispondenza della zona di interesse e delle impostazioni definite in locale, su un apposito controller, oppure online attraverso una web-app installata proprio sul controller.

# Funzionamento
Il funzionamento del progetto si sviluppa du 3 piani differenti che vengono integrati attraverso la scheda _controller_:
1. **Stazione meteo esterna**
2. **Controller locale**
3. **Web-app**

# 1-Stazione meteo esterna
La _stazione meteo esterna_ ha il compito di rilevare e identificare le attuali condizioni meteo e di comunicarle alla _scheda controller_.
Per fare ció é dotata di 3 diversi sensori:
- Sensore di umiditá DH11: questo sensore ha il compito di rilevare temperatura ed umiditá dell'ambiente circostante, comunica in modo digitale con la scheda,
- Sensore di rilevamento pioggia/neve: permette di rilevare una precipitazione e stimarne la sua entitá,
- Fotoresistenza: attraverso essa si puó misurare la luminositá esterna rilevando presenza di nuvole o meno.
Questi due sensori, essendo entrambi analogici, competono per l'utilizzo dell'unico ingresso analogico della scheda pertanto vengno attiviti alternativamente per poter sfruttare la stessa porta, per evitare che una rilevazione "sporchi" la successiva sono stati utilizzati 2 diodi che permettono il passaggio della corrente solo verso il pin e non vieceversa.
La combinazione dei dati rilevati permette di identificare, con un margine di errore, le attuali condizioni meteo che sono state cosí definite:
- Sunny: nel caso non vengano rilevate precipitazioni e non vi siano nuvole
- Cloudy: nel caso non vengano rilevate precipitazione ma la luminusitá ambientale sia bassa
- Rainy: nel caso vengano rilevate precipitazioni basse
- Storm: nel caso vengano rilevate precipiatazioni alte
- Snow: nel caso vengano rilevate precipitazione e la temperatura sia sotto lo 0 celsius
Questi dati vengono comunicati al _controller_ attraverso una form HTTP, infatti la stazione é a tutti gli effetti un client agli occhi del server.
Una volta inviati i dati si pone in stato di sleep per 30 minuti in modo da ridurre il piú possibile il consumo di batteria.

# 2-Web server/Controller locale
La scheda _controller_ svolge 3 funzioni:
1. Gestire l'irrigazione sulla base dei segnali ricevuti dalla stazione e dalle impostazioni 
2. Gestire il web-server per la ricezione dei segnali dalla stazione e dalla pagina web
3. Gestire il setting delle impostazioni in locale
Sulla scheda é installato il web-server che permette la comunicazione con la stazione meteo e con i diversi utenti.
La pagina web permette di avere un overview sulla sitazione meteo mostrando temperatura, umiditá e condizioni climatiche.
Per l'interazione in locale con l'utente sono usati un pulsante, un rotary encoder ed uno schermo oled che permettono di avviare un irrigazione oppure di settarne una giornaliera, setttimanale o di eliminare tutte quelle in programma.
Queste operazioni possono essere fatte anche attraverso la paginaweb in cui é presente la funzionalitá aggiuntiva di impostare un singola irrigazione per un dato giorno ad una data ora.
Il _controller_ si occupa inoltre di gestire l'irrigazione a seconda del meteo bloccandola quando le condizioni non la richiedono o al contrario avviandola quando necessario.

# Screenshot della webpage
![EmbeddedWebPage](https://user-images.githubusercontent.com/80100758/189540844-6766806b-bf1d-43cd-a6f8-cd9c628c7029.PNG)
# Schema stazione meteo
![StazioneMeteo_bb](https://user-images.githubusercontent.com/80100758/189505529-eb11b7ca-12f4-41bf-a35c-8edfaba1200a.png)
# Schema web server/controller locale
![Server_bb](https://user-images.githubusercontent.com/80100758/189505564-763bce01-ac2f-4f7a-a075-e047e7184bd9.jpg)


