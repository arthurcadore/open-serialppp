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

Implementar enquadramento e envio de mensagem entre TX / RX 
Implementar a aplicação para que o enquandramento a consuma 
Ambos passos anteriores precisam ser estruturados com poller
Implementar o CRC-16

Implementar o controle por subcamada
```