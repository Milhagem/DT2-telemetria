# Configuracao User_Setup.h

Para poder usar o codigo "displayTFT.ino" corretamente, eh necessario configurar o arquivo User_Setup.h **instalado nas bibliotecas do seu computador** corretamente. Isto eh, nao basta configurar o User_Setup.h que esta neste diretorio, voce precisar ir ate onde as bibliotecas de Esp32 estao no seu PC, copiar o arquivo User_Setup.h que estah nessa pasta e colar no lugar do arquivo do seu PC.

Para isso, siga o seguinte passo-a-passo:

* 1 - Copie todo o conteudo de User_Setup.h que esta dentro desta pasta
* 2 - Em displayTFT.ino, procure pelo #include <TFT_eSPI.h> (linha 17) e deh um Ctrl Click nele.
* 3 - Dentro do arquivo TFT_eSPI.h, procure pelo #include <User_Setup_Select.h> (linha 68) e deh um Ctrl Click nele.
* 4 - Dentro de User_Setup_Select.h, procure por #include <User_Setup.h> (linha 30) e deh um Ctrl Click nele.
* 5 - Substitua todo o conteudo do arquivo deste User_Setup.h pelo conteudo que vc copiou.


## A titulo de curiosidade...
Estas sao as diferencas entre o arquivo padrao User_Setup.h que vc instala da Arduino IDE; e o User_Setup.h configurado para a placa t-sim7000g:

**A linha 12 é comentada:**<br>
`//#define USER_SETUP_INFO "User_Setup"`

**As linhas 169 - 171 são comentadas:**<br>
`//#define TFT_CS   PIN_D8  // Chip select control pin D8`<br>
`//#define TFT_DC   PIN_D3  // Data Command control pin`<br>
`//#define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)`

**As linhas 206 - 211 não são comentadas:**<br>
`#define TFT_MISO 19`<br>
`#define TFT_MOSI 23`<br>
`#define TFT_SCLK 18`<br>
`#define TFT_CS    5  // Chip select control pin`<br>
`#define TFT_DC   32  // Data Command control pin`<br>
`#define TFT_RST  33  // Reset pin (could connect to RST pin)`<br>