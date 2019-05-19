<b>IMPORTANTE:</b>

Todos os programas deste repositório usam
a lib <code>ucontext.h</code>, que está 
descontinuada no MacOS desde o MacOS 10.6. Portanto,
é necessário definir _XOPEN_SOURCE *antes de todos os includes
e definições*:

<code> // habilita compatibilidade POSIX no MacOS X (para ucontext.h) </code>

<code> #define _XOPEN_SOURCE 600 </code>

<h1>Trabalho ZERO de Sistemas Operacionais 2019/1 </h1>

Fila para treinar manipulação de ponteiros.

Para rodar no MacOS: 

<code> clang testafila.c queue.c -o testafila -Wall -DDEBUG </code>

<code> chmod +x testafila </code>

<code> ./testafila </code>

<h1>Trabalho 1 (p1) de Sistemas Operacionais 2019/1 </h1>
  
Trocas de contexto

Para rodar no MacOS:

<code> clang context.c -o context -Wall -DDEBUG </code>

<code> ./contex </code>

<h1>Trabalho 2 (p2) de Sistemas Operacionais 2019/1 </h1>
  
Trocas de contexto com funções encapsuladas.
O professor deu o arquivo <code>ppos.h</code>
pronto, e a tarefa era implementar as funções de
<code>ppos.h</code> no arquivo <code>ppos.c</code>
e compilar junto com seu arquivo de testes para
efetuar a execução. 

Para rodar no MacOS:

<code> clang pingpong-tasks1.c ppos.c -o pingpong-tasks1 -Wall -DDEBUG </code>

<code> ./pinpong-tasks1 </code>
