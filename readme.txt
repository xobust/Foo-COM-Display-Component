This component demonstrates:
* main.cpp :
  * Info om componenten.
* preferences.cpp :
  * Deklarerar inställningsvariabler
  * Hanterar inställningsdialågen
* COM.cpp 
  * Hanterar vilken com information som ska skickas och när
* Serial.cpp
  * Sköter det praktiska för den seriella komunikationen
* initquit.cpp :
  * Det som händer när programmet startar

Avalible Baud Rates!
1200, 2400, 4800, 9600, 19200, 38400 and 76800

Sign information format
~[SignID]~f01[Mode]\b\s{TEXT}

Colors:
\a RED
\b BRIGHT RED 
\c ORANGE 
\d BRIGHT ORANGE 
\e YELLOW 
\f BRIGHT YELLOW
\g GREEN 
\h BRIGHTGREEN 
\i LAYERMIX 
\j BRIGHT LAYERMIX 
\k VERTICAL MIX 
\l SAWTOOTH MIX 
\m GREENON RED 
\n RED ON GREEN 
\o ORANGE ON RED 
\p Yellow on Green

Mode:
A Cyclic
B Imidate
C Open From Right
D Open from Left
E Open From Center
F Open To Center
G Cover From Center 
H Cover From right
I Cover from left
J Cover to center
K Scroll up 
L Scroll down
M Interlace to center
N Interlace cover 
O Cover Up
P Cover Down
Q Scanline
R Explode
S Pacman
T Fall & Stack
U Shoot
V Flash
W Random
X Slide IN


\s ???

\Y[n] Speed
\Z[n] Stay


Set Time command format:
~128~EYYYYMMDDHHMMSS