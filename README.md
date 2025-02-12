# open-serialppp

protocolo de enlace ponto-a-ponto, para camada física do tipo UART
encapsulamento de mensagens com até 1024 bytes
recepção de mensagens livres de erros
garantia de entrega
controle de acesso ao meio
conectado (estabelecimento de sessão)


TODO 
```
 ALTERAR O CMAKE

Implementar serializar o quadro 
calcular CRC em cima do quadro serializado 
adicionar delimitadores
enviar quadro

receber quadro e verificar CRC
deserializar quadro
validar ACK para adicioar o proximo sequence number 

```