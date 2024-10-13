# Aplikace pro získání statistik o síťovém provozu

## Autor: Lukáš Píšek (xpisek02)
## Datrum vytvoření: 13.10.2024

### Popis
Aplikace sloužící pro výpis aktuálních přenosových rychlostí pro jednotlivé komunikující IP adresy na zadaném rozhraní. Program ihned po spuštění začne chytat pakety na daném rozhraní a za pomocí knihovny `ncurses` z nich začne vypisovat informace:
- Zdrojová IP adresa
- Cílová IP adresa
- Pokud je daný paket typu TCP nebo UDP, tak také zdrojové a cílové porty
- Protokol, který paket využívá
- Přijaté a odeslané data
  - Rychlost připojení v b/s
  - Počet paketů za sekundu

### Spouštění aplikace
Aplikace využívá pro svůj běh knihovny `libpcap` a `ncurses`. Tyto knihovny se na Ubuntu instalují pomocí příkazů
```sh
sudo apt install libpcap-dev
```
```sh
sudo apt install ncurses-dev
```

Poté se aplikace vytvoří za pomocí příkazu
```
make
```

Následně se samotná aplikace spustí příkazem 
```sh
sudo ./isa-top -i interface [-s b|p]
```
Argument `-i interface` vybere rozhraní, na kterém bude aplikace odchytávat pakety.
Volitelný argument `-s b|p` vybere podle čeho se budou vypsané komunikace třídit. Spuštění programu s argumentem `-s b` bude nejprve vypisovat připojení s nějvětším pohybem dat. Argument `-s p` vypíše nejdříve připojení s největším počtem pohybujících se paketů. Při spuštění programu bez tohoto argumentu se budou připojení vypisovat identicky jako při použití `-s b`.

### Soubory
- ArgParser.cpp
- ArgParser.h
- MainClass.cpp
- Makefile
- Output.cpp
- Output.h
- PacketCapturinng.cpp
- PacketCapturinng.h
- PacketInfo.h
- PacketParser.cpp
- PacketParser.h