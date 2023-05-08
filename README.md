# PNRF-ATP-Comunication
Software para inserção de arquivos .pnrf em simulações do ATP 

# Descrição para quem vai utilizar o codigo (passo a passo)
O software utiliza bibliotecas do Qt para fazer a interface grafica. Foi utilizado a versão 6.5 do Qt. Tambem foi utilizado o Qt Creator e o Qt Design para auxiliar na criação da interface.

Foi utilizado o compilador MSVC2019. (tive problemas para importar uma dll usando o MINGW, e achei mais facil usar o msvc que permite usar o comando import)

Tambem foi utilizado a "qcustomplot" para plotar o grafico do sinal desejado. (esse grafico auxilia para visualização da oscilografia)
A inclusão da "qcustomplot" é atravez dos arquivos .cpp e .h fornecidos em: https://www.qcustomplot.com/

Recursos de dll fornecidos pela HBM são necessarios para leitura de arquivos ".pnrf". Para isso deve ser instalado o "PNRF Reader Toolkit" fornecido em: https://www.hbm.com/en/2475/support-downloads-perception/

# Arquivo instalador
Caso pretenda apenas usar o software, na pasta instalador, é fornecido o instalador do programa. 
É importante lembrar que é necessario instalar o "PNRF Reader Toolkit" para que o programa funcione. (https://www.hbm.com/en/2475/support-downloads-perception/)

# Reconstrução do ATP
Para o programa poder se comunicar com o ATP, é necessario recompilar o ATP para poder fazer a comunicação.
Na pasta "ATP" encontrasse todos os arquivos necessarios e tambem um arquivo Read-me para detalhar o procedimento.
