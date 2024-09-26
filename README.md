# Controle de Portão com Máquina de Estados e Emergência

Este projeto simula o controle de um portão de garagem utilizando uma **máquina de estados** implementada em C. O sistema suporta comandos manuais de **ABRIR**, **FECHAR**, além de um comando de **EMERGÊNCIA** que pode ser ativado a qualquer momento para interromper o movimento do portão. O programa é capaz de lidar com múltiplas threads para monitorar os comandos do usuário e gerenciar o controle do portão de forma simultânea.

## Funcionalidades

- **Abrir o Portão**: O portão pode ser aberto manualmente com o comando `ABRIR`, o que inicia o processo de abertura, levando 10 segundos.
- **Fechar o Portão**: O portão pode ser fechado manualmente com o comando `FECHAR`, também levando 10 segundos.
- **Emergência**: A qualquer momento, o comando `EMERGENCIA` pode ser acionado, interrompendo o movimento do portão. O portão ficará parado até que um novo comando (`ABRIR` ou `FECHAR`) seja inserido.
- **Estados**:
  - **Fechado**: Estado inicial do portão.
  - **Aberto**: Estado quando o portão está totalmente aberto.
  - **Abrindo**: Estado durante o processo de abertura.
  - **Fechando**: Estado durante o processo de fechamento.
  - **Emergência**: Estado quando o portão é interrompido por uma emergência.
