Como Rodar o Projeto

Passo 1: Compilar e rodar o satélite (QEMU)
Compilação do código do satélite: Navegue até o diretório onde o código do satélite está localizado e rode o comando para compilar o código:

wsl
make clean && make
qemu-system-arm -M lm3s6965evb -nographic -monitor pty -serial stdio -kernel maria-sat.elf -nic none | tee logs.txt
*logs.txt tem que estar no ficheiro 'interface'


Passo 2: Rodar o servidor Flask (para exibir a interface)
Navegue até o diretório da interface: Vá até a pasta interface onde os arquivos HTML, CSS e JavaScript estão localizados.

Rodar o servidor Flask: No terminal, execute o comando abaixo para iniciar o servidor Flask:

wsl
cd interface
source venv/bin/activate
python3 server.py

Acessar a interface:

Abra um navegador web e vá para http://localhost:5000 para visualizar a interface do painel de processos do satélite.

A interface vai buscar e exibir os logs em tempo real, atualizando a cada 2 segundos.

Passo 3: Acessar os logs (API)
A interface web acessa os logs através da rota /logs, que retorna as últimas 15 linhas do arquivo logs.txt sempre que é chamada.

A interface também exibe o status dos processos do satélite na API /api/status, onde os dados são simulados, mas podem ser atualizados conforme sua necessidade.

Observações
QEMU simula o comportamento do satélite e gera os logs no arquivo logs.txt. Esses logs são então lidos e exibidos na interface web.

O servidor Flask serve a interface HTML, CSS e JavaScript para que você possa visualizar o painel dos processos.

Problemas Conhecidos
Se os logs não estão aparecendo na interface, verifique se o arquivo logs.txt contém dados válidos e se o Flask está conseguindo ler o arquivo corretamente.



