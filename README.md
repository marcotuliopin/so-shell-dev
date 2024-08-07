## Parte 1

### Tarefa 1

Implementamos o código para executar comandos simples através do uso da função execvp(). Essa função foi escolhida por suportar um vetor de argumentos como
segundo parâmetro. Para o seu primeiro parâmetro computamos uma string chamada de "cmdpath", que contém o caminho para o comando de shell a ser executado ("/bin/"+<nome do comando>).
Como segundo parâmetro, usamos os argumentos captados na estrutura de dados dada "ecmd->argv".

### Tarefa 2

Implementamos o código para executar comandos de redirecionamento. Para isso, foi preciso redirecionar a entrada/saída especificada na estrutura
"rcmd->fd" para o arquivo requisitado (especificado em "rcmd->file"). Abrimos o arquivo usando a função open(), no modo "rcmd->mode" e o redirecionamento da stream
de entrada/saída foi feito usando a função dup2.

### Tarefa 3

Implementamos o código para executar pipes. Criamos nosso pipe "p" por meio da função pipe(). Para fazer a comunicação entre processo pai e filho, fazemos um fork do processo
atual usando fork(). Alteramos o fluxo de execução entre o processo pai e o processo filho. Para criar o canal de comunicação, o processo pai fecha sua stream de leitura do pipe e redireciona
sua stream padrão de escrita para a do pipe. Após isso, o processo pai é executado. Enquanto isso, o processo filho, após esperar a execução do pai, fecha sua stream de escrita do pipe e
redireciona sua stream padrão de leitura para a do pipe. Por fim, o processo filho também é executado. Um erro é levantado caso a operação de fork não seja bem sucedida.


## Parte 2

Primeiramente, criamos a função print_process_info em que recebemos um PID acessamos o arquivo de /proc/PID/status/, lemos as linhas que começam com Uid, Name e State para adquirir o id de Usuário, que mais depois será utilizado para identificar o usuário, o nome e o estado. Nessa mesma função, utilizamos a função getpwuid da bibilioteca pwd para descobrir o nome do usuário e realizamos o print desses dados formatados.
Com essa função pronta, podemos finalizar a primeira parte do nosso top, criando a função list_process em que percorremos os diretórios dentro de proc e utilizamos print_process_info para os primeiros 20 PIDs disponíveis. Esse processo é repetido a cada um segundo, garantindo que a lista apresentada esteja atualizada.
Para o envio de sinais, precisamos adicionar uma nova função send_signal em que realizamos a leitura de um PID e do sinal que gostaria de ser enviado e utilizando a função kill enviamos esse sinal.
Com as funções list_process e send_signal podemos criar uma thread para cada uma dessas funções e permitir seu funcionamento em paralelo. Desse modo, ambas as funcionalidades requeridas nesse programa estão disponíveis e funcionais simultaneamente.
