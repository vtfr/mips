# Concepção Estruturada de Circuitos Integrados: Arquitetura MIPS
Aluno: Victor Franco Vieira Lima, 2017.

## Introdução

A arquitetura *MIPS*, que significa *Microprocessor without Interlocked Pipeline Stages*, isto é, **Microprocessador sem Estágios intertravados de Pipeline**, desenvolvida pela *MIPS Computer Systems* em 1985, foi uma das principais arquiteturas **RISC** e **Big-Endian** de sua época, marcando uma geração inteira de processadores que a sucederam.

Neste relatório exploraremos um pouco do subconjunto MIPS definido no livro **Computer Organization and Design, MIPS Edition** (ISBN: 978-0124077263), por David A. Patterson e‎ John L. Hennessy, implementando-a em *VHDL* com suite de testes em C, usando o pacote *Alliance*.

## RISC

Uma arquitetura *RISC*, isto é, *Reduced Instruction Set Computer*, **Computador com Conjunto Reduzido de Instruções**, consiste em uma arquitetura em que as operações serão realizadas apenas em registradores e sem microcódigo, portanto. Por ser baseado em registrador, qualquer operação com um dado na memória deve ser primeiro carregada em um registrador por uma instrução de busca para só depois ser processado, diferente dos CISCs como o x86, que podem fazer a busca na memória, processamento e escrita implicitamente em uma só instrução.

## Endianness

Antes de entender o conceito de Endianness, que significa **Ordenação**, primeiro temos que entender o conceito de byte mais significativo.

Vamos usar a palavra de quatro bytes *0x000F4240* (1 milhão em decimal) por exemplo. Separando seus bytes (a cada dois caracteres em hexadecimal temos um byte), temos:

| Byte 0 | Byte 1 | Byte 2 | Byte 3 |
| ------ | ------ | ------ | ------ |
| 0x00   | 0x0F   | 0x42   | 0x40   |
| *MSB*  |        |        | *LSB*  |

Como podemos ver, o byte mais significativo (*MSB*) é o primeiro, porque qualquer mudança nele resultará em uma mudança drástica no resultado final, enquanto o byte menos significativo (*LSB*) é o último.

Uma forma fácil de ver isso e experimentadno incrementar o MSB em comparação ao LSB. Se incrementarmos o MSB, temos a palavra *0x010F4240*, que equivale a mais de 17 milhões em decimal. Muito diferente de incrementarmos o LSB, que teríamos *0x000F4241*, 1 milhão e 1 em decimal.

Agora que entendemos o conceito de byte mais significativo, existem duas formas de se armazenar um valor na memória: a **Big-Endian**, que começa a armazenar o valor pelo byte mais significativo primeiro e **Little-Endian**, que começa pelo byte menos significativo, na ordem inversa. O número 1.000.000 (0x000F4240), se fossemos armazená-lo no endereço 30, por exemplo, teríamos:

| Endereço de Memória | Valor Big-Endian | Valor Little-Endian |
| ------------------- | ---------------- | ------------------- |
| 30                  | 0x00             | 0x40                |
| 31                  | 0x0F             | 0x42                |
| 32                  | 0x42             | 0x0F                |
| 33                  | 0x40             | 0x00                |

No caso do nosso processador MIPS, é usado a ordenação **Big-Endian**.

## Instruções

As instruções do processador MIPS estudado são divididas em três categorias: **Tipo R**, **Tipo I** e **Tipo J**.

### Tipo R

As instruções Tipo R são





### Diagrama do Somador/Subtrator de 4 Bits com Acumulador

![Diagrama](schematic.png)

O circuito possui apenas três entradas, que são a entrada de valores de 4 bits `A` e os os seletores de modo de operação `sel0` e `sel1`, com 1 bit cada, e duas saídas, que são a saída de 4 bits da operação atual `S` e o indicador de overflow
`Cout`, que é ativado toda vez que uma operação resulta em um número maior que 4 bits.

| sel0 | sel1 | Resposta                         | Operação         |
|------|------|----------------------------------|------------------|
| 0    | 0    | Copia A para ACC                 | `ACC <= A`       |
| 0    | 1    | Soma A com ACC e grava em ACC    | `ACC <= ACC + A` |
| 1    | 0    | Copia Inv(A) para ACC            | `ACC <= Inv(A)`  |
| 1    | 1    | Subtrai A de ACC e grava em ACC  | `ACC <= ACC - A` |


### Organização do Projeto

A organização do código foi feita de acordo com o livro *A Top Down Approach To IC Design*, escritp por Chris Browy, Glenn Gullikson e Mark Indovina, onde temos a seguinte estruturação do código:

* `bin`: Arquivos binários e scripts para a automatização das etapas de produção e verificação do circuito
* `etc`: Arquivos de configuração do simulador
* `include`: Arquivos de código compartilhados
* `src`: Arquivos de código
  * `c`: Código em C
  * `vhdl`: Código em VHDL
* `test`: Arquivos resultantes de testes, como tabelas verdade
* `work`: Arquivos sendo trabalhados por scripts


### Documentação do Projeto

A Documentação do código está escrita no próprio código através de comentários antes de cada operação importante. Para fins de conveniência os trechos mais importantes de código serão copiados e colados aqui conforme a necessidade deles.


## Modelo de Referência de Ouro

O Modelo de Referência de Ouro, do inglês *Golden Reference Model*, trata-se de um modelo de referência em alto nível de um dado circuito capaz de descrever seu comportamento, isto é, suas saídas, em função das suas entradas. Esse modelo é
usado para produzir um vetor de casos de teste que exaustivamente compara se o resultado produzido pelo circuito está de acordo com o vetor de testes gerado.

Neste trabalho, fizemos um modelo em alto-nível em C99, que, uma vez executado, gera os vários vetores de teste para cada um dos componentes do circuito.
