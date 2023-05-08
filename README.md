# PNRF-ATP-Comunication
Software para inserção de arquivos .pnrf em simulações do ATP 

# Descrição para quem vai utilizar o codigo (passo a passo)
O software utiliza bibliotecas do Qt para fazer a interface grafica. Foi utilizado a versão 6.5 do Qt. Tambem foi utilizado o Qt Creator e o Qt Design para auxiliar na criação da interface.

Foi utilizado o compilador MSVC2019. (tive problemas para importar uma dll usando o MINGW, e achei mais facil usar o msvc que permite usar o comando import)

Tambem foi utilizado a "qcustomplot" para plotar o grafico do sinal desejado. (esse grafico auxilia para visualização da oscilografia)
A inclusão da "qcustomplot" é atravez dos arquivos .cpp e .h fornecidos em: https://www.qcustomplot.com/

Recursos de dll fornecidos pela HBM são necessarios para leitura de arquivos ".pnrf". Para isso deve ser instalado o "PNRF Reader Toolkit" fornecido em: https://www.hbm.com/en/2475/support-downloads-perception/

Os arquivos de projeto encontran-se na pasta PNRF_ATP, sendo:
  * O arquivo .olb e .dll arquivos fornecidos pela HBM e tambem podem ser encontrados em C:\Program Files\Common Files\HBM\Components após a instalação do toolkit.
  * Os arquivos qcustomplot.cpp e qcustomplot.h são arquivos fornecidos pela biblioteca qcustomplot e não devem ser alterados.
  * stdafx.h e stdafx.cpp arquivos adicionais que não precisam ser alterados.
  * mainwindow.ui é o arquivo de interface, usado para simplificar a criação da interface.
  * mainwindow.cpp e mainwindow.h contem o codigo principal com todas as funçoes implementadas.
  * main.cpp é usado como padrão para criar a aplicação que esta em mainwindow.

# Arquivo instalador
Caso pretenda apenas usar o software, na pasta instalador, é fornecido o instalador do programa. 
É importante lembrar que é necessario instalar o "PNRF Reader Toolkit" para que o programa funcione. (https://www.hbm.com/en/2475/support-downloads-perception/)

Tambem é necessario recompilar o ATP para adicionar uma função que permite a comunicação.

# Reconstrução do ATP
Para o programa poder se comunicar com o ATP, é necessario recompilar o ATP para poder fazer a comunicação.
Na pasta "ATP" encontrasse todos os arquivos necessarios e tambem um arquivo Read-me para detalhar o procedimento.

# Simulação de casos de FAI
* O objetivo da construção desse software foi a simulação de testes de ignição em vegetação (https://discover.data.vic.gov.au/dataset/powerline-bushfire-safety-program-vegetation-conduction-ignition-test-report). 
* Para realizar essas simulaçoes deve ser carregado um arquivo .pnrf (contendo a oscilografia) no Software. Na janela principal, clique em load PNRF e selecione o arquivo.
* Apos carregar o arquivo, será exibido na tela algumas informações sobre o arquivo. O tempo do ATP mostrado é o tempo necessario na simulação do ATP para acomodar toda oscilografia.
* Abra um sistema no ATP para realizar a simulação e configure o tempo maximo e o timestep desejado (o timestep pode ser qualquer um, desde que maior que o da oscilografia).
* No exemplo fornecido em ATP/Exemplo, é utilizado um bloco do models junto com uma fonte de corrente para realizar a simulação da inserção da oscilografia. Copie todo esse conjunto para o sistema desejado. Este bloco do models fornece informação de tensão e recebe informação de corrente do Software PNRF_ATP.
* É importante lembrar que é possivel alterrar a frequencia e a amplitude da oscilografia fornecida no .pnrf modificando os valores no menu ATP/Settings.
* É importante lembrar que o software apenas insere a corrente de LF.
* A inserção da corrente ocorre pela sincronização da tensão fornecida ao bloco models e a tensão na oscilografia.
