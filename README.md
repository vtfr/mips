# Concepção Estruturada de Circuitos Integrados: Arquitetura MIPS
Aluno: Victor Franco Vieira Lima, 2017.

## Introdução

A arquitetura *MIPS*, que significa *Microprocessor without Interlocked Pipeline Stages*, isto é, **Microprocessador sem Estágios intertravados de Pipeline**, desenvolvida pela *MIPS Computer Systems* em 1985, foi uma das principais arquiteturas **RISC** e **Big-Endian** de sua época, marcando uma geração inteira de processadores que a sucederam.

Neste relatório exploraremos um pouco do subconjunto MIPS definido no livro **Computer Organization and Design, MIPS Edition** (ISBN: 978-0124077263), por David A. Patterson e‎ John L. Hennessy, implementando-a em *VHDL* com suite de testes em C, usando o pacote *Alliance*.

### Organização do Projeto

A organização do código foi feita de acordo com o livro **A Top Down Approach To IC Design** (ISBN: 9780128007723), por Chris Browy, Glenn Gullikson e Mark Indovina, onde temos a seguinte estruturação do código:

* `bin`: Arquivos binários e scripts para a automatização das etapas de produção e verificação do circuito
* `etc`: Arquivos de configuração do simulador
* `include`: Arquivos de código compartilhados
* `src`: Arquivos de código
  * `c`: Código em C
  * `vhdl`: Código em VHDL
* `test`: Arquivos resultantes de testes, como tabelas verdade
* `work`: Arquivos sendo trabalhados por scripts

O código completo assim como uma cópia desse documento podem ser obtidas no link https://github.com/vtfr/mips.

### Documentação do Projeto

A Documentação do código está escrita no próprio código através de comentários antes de cada operação importante. Para fins de conveniência os trechos mais importantes de código serão copiados e colados aqui conforme a necessidade deles.

### Teste

O teste do código pode ser feito executando o seguinte comando no diretório raiz do projeto:

```
$ make test
```

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

## Registradores

O MIPS possui 32 registradores de 32 bits cada, entre os quais o primeiro, chamado de `$s0` é fixo no valor zero, isto é, não pode ser sobrescrito, apenas lido.

Por ser uma arquitetura **RISC**, todas as operações devem ser feitas em função dos registradores, que são a base dessa arquitetura.

A lógica dos registradores está descrita no arquivo [`regfile.vhdl`](src/vhdl/regfile.vhdl), sendo validada pelo arquivo [`regfile.c`](src/c/regfile.c) que gera a suite de testes.

## Instruções

### Listagem

As instruções do subset MIPS que trabalharemos são as seguintes:

| Categoria          | Instrução                          | Exemplo              | Comentário                                                          |
|--------------------|------------------------------------|----------------------|---------------------------------------------------------------------|
| Aritmetica         | add                                | `add $s1, $s2, $s3 ` | Adiciona `$s3` a $s2` e armazena o resultado em `$s1`               |
| Aritmetica         | sub                                | `sub $s1, $s2, $s3`  | Subtrai `$s3` de $s2` e armazena o resultado em `$s1`               |
| Dados     | Lê palavra      | `lw $s1, L($2)`  | Lê a palavra no endereço `$s2 + L` para `$s1` |
| Dados     | Escreve Palavra | `sw $s1, L($s2)` | Escreve a palavra `$s1` no endereço `$s2 + L` |
| Lógica             | and                                | `and $s1, $s2, $s3`  | Realiza AND em `$s2` e `$s3` e armazena o resultado em `$s1`        |
| Lógica             | or                                 | `or $s1, $s2, $s3`   | Realiza OR em `$s2` e `$s3` e armazena o resultado em `$s1`         |
| Lógica             | nor                                | `nor $s1, $s2, $s3`  | Realiza NOR em `$s2` e `$s3` e armazena o resultado em `$s1`        |
| Lógica             | and imediato                       | `andi $s1, $s2, L`   | Realiza OR em `$s2` e `L` e armazena o resultado em `$s1`           |
| Lógica             | or imediato                        | `ori $s1, $s2, L`    | Realiza OR em `$s2` e `L` e armazena o resultado em `$s1`           |
| Lógica             | shift pra esquerda                 | `sll $s1, $s2, 10`   | Realiza shift em `$s2` por `10` para a esquerda e armazena em `$s1` |
| Lógica             | shift pra direita                  | `srl $s1, $s2, 10`   | Realiza shift em `$s2` por `10` para a direita e armazena em `$s1`  |
| Branching          | branch caso igual                  | `beq $s1, $s2, L`    | Pula para endereço `L` se `$s1 == $s2`                              |
| Branching          | branch caso não igual              | `bne $s1, $s2, L`    | Pula para endereço `L` se `$s1 != $s2`                              |
| Branching          | atribui teste se negativo          | `slt $s1, $s2, $s3`  | Atribui `1` em `$s1` caso `$s2 < $s3`. Caso contrário atribui `0`   |
| Branching          | atribui teste se negativo imediato | `slt $s1, $s2, 100`  | Atribui `1` em `$s1` caso `$s2 < 100`. Caso contrário atribui `0`   |
| Jump Incondicional | jump                               | `jump L`             | Pula para o endereço `L`                                            |

As instruções do processador MIPS estudado são divididas em três categorias: **Tipo R**, **Tipo I** e **Tipo J**, que são relacionadas a forma como a instrução é armazenada na memória, isto é, seu formato binário.

### Tipo R

As instruções tipo R são caracterizadas por usarem três registradores: sendo dois para entrada de dados e um para armazenamento. São divididas em seis campo:

* O número inteiro equivalente ao operador, chamado `op`;
* O número do registrador de saída `rs`;
* O número do registrador de entrada `rt`;
* O número do registrador de entrada `rd`;
* O shift para esquerda `shamt` (usado para operações com shift).
* A função `funct` que é enviada a ULA como complemento ao operador;

| Operador `op` | Registrador Saída `rs` | Registrador Entrada 1 `rt` | Registrador Entrada 2 `rd` | Shift para esquerda `shamt` | Função `funct` |
|---------------|------------------------|----------------------------|----------------------------|-----------------------------|----------------|
| 6 bits        | 5 bits                 | 5 bits                     | 5 bits                     | 5 bits                      | 6 bits         |

Todas as operações do tipo R possuem `op` zero, deixando a função específica que realizam para o parâmetro `funct`, como mostrado acima.


### Tipo I

As instruções do tipo I, chamadas de **imediatas**, são instruçẽos que, ao invés de terem dois registradores para entrada, possuem um registrador para entrada e um campo onde um inteiro será armazenado diretamente na instrução, para ser usado por ela, chamado `imm`, que possui 16 bits. A operação é determinada unicamente pelo opcode `op`, diferente das instruções do **Tipo R**.

| Operador `op` | Registrador Saída `rs` | Registrador Entrada `rt` | Valor Imediato `imm` |
|---------------|------------------------|--------------------------|----------------------|
| 6 bits        | 5 bits                 | 5 bits                   | 16 bits              |


### Tipo J

O tipo J é feito exclusivamente para instruções de salto incondicional `jump`, possuindo um inteiro `L` de 26 bits que representa a distância (positiva ou negativa) do salto em relação ao endereço da próxima instrução depois do `jump`.

| Operador `op` | Endereço `L` |
|---------------|--------------|
| 6 bits        | 26 bits      |

Devido ao fato de que no MIPS cada instrução possui 32 bits, ou seja, 4 bytes, todos os saltos devem ser alinhados à 4 bytes, de tal forma que as instruções se localizem nos endereços multiplos de 4. Sabendo disso e do fato de que todos os números múltiplos de 4 possuem os dois bits menos significativos `00`, o valor `L` é armazenado sem esses últimos dois bits, para garantir uma distância maior de salto possível. Lembrando que o shift para a esquerda é equivalente a multiplicação por quatro.

Também é necessário notar que o valor é complemento de dois, portanto não sendo possível somar dois valores complemento de dois de tamanho de bits diferentes e obter sempre um resultado correto. Para isso, é necessário extender o valor de 28 bits (26 bits de `L` mais os últimos `00`) para um valor de 32 bits, que, no MIPS, chamamos de `signextend`.

O cálculo final para o endereço fica:

```
PC := (PC + 4) + signextend(L << 2)
```

## Multi-Ciclo

A arquitetura Multi-Ciclo é uma otimização a nível de hardware que permite a reutilização de componentes através da execução de instruções em ciclos menores. Cada instrução será dividida em uma série de etapas que serão executadas em vários ciclos menores, possibilitando que componentes que normalmente estariam ociosos e teriam de ser duplicados para funcionarem em um ciclo só possam ser reaproveitados para executar determinadas tarefas da instrução.

Para isso, é necessário ter uma unidade de controle que é capaz de armazenar o estado atual durante vários ciclos e controlar o comportamento dos outros componentes, chamada de controle. O controle é uma **Máquina Finita de Estados** (FSM) que está descrita em [control.vhdl](src/vhdl/control.vhdl) e pode ser validada em [control.c](src/c/control.c).

Abaixo um trecho do arquivo gerado pelo genpat que serve como teste da unidade de controle:

```
-- description generated by Pat driver

--			date     : Fri Nov 24 18:56:16 2017
--			revision : v109

--			sequence : control

-- input / output list :
in       clk B;;
in       reset B;;
in       opcode (6 downto 0) X;;
in       funct (5 downto 0) X;;
out      memtoreg B;;
out      regdst B;;
out      iord B;;
out      pcsrc (1 downto 0) X;;
out      aluop (1 downto 0) X;;
out      alusrca B;;
out      alusrcb (1 downto 0) X;;
out      aluctrl (2 downto 0) X;;
out      irwrite B;;
out      memwrite B;;
out      pcwrite B;;
out      regwrite B;;
out      branch B;;
in       vdd B;;
in       vss B;;

begin

-- Pattern description :

--                                c r o  f   m  r  i  p  a  a  a  a  i  m  p  r  b v v  
--                                l e p  u   e  e  o  c  l  l  l  l  r  e  c  e  r d s  
--                                k s c  n   m  g  r  s  u  u  u  u  w  m  w  g  a d s  
--                                  e o  c   t  d  d  r  o  s  s  c  r  w  r  w  n      
--                                  t d  t   o  s     c  p  r  r  t  i  r  i  r  c      
--                                    e      r  t           c  c  r  t  i  t  i  h      
--                                           e              a  b  l  e  t  e  t         
--                                           g                          e     e         


-- Beware : unprocessed patterns

<          0 ps>Reset_0         : 0 1 00 00 ?0 ?0 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          1 ps>                : 0 0 00 24 ?0 ?0 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          2 ps>FETCH_2         : 1 0 00 24 ?0 ?0 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          3 ps>                : 0 0 00 24 ?0 ?0 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          4 ps>DECODE_4        : 1 0 00 24 ?0 ?0 ?0 ?0 ?0 ?0 ?3 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          5 ps>                : 0 0 00 24 ?0 ?0 ?0 ?0 ?0 ?0 ?3 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          6 ps>EXECUTE_6       : 1 0 00 24 ?0 ?0 ?0 ?0 ?2 ?1 ?0 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          7 ps>                : 0 0 00 24 ?0 ?0 ?0 ?0 ?2 ?1 ?0 ?0 ?1 ?0 ?1 ?0 ?0 1 0 ;
<          8 ps>ALUWRITEBACK_8  : 1 0 00 24 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<          9 ps>                : 0 0 00 25 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         10 ps>FETCH_10        : 1 0 00 25 ?0 ?1 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         11 ps>                : 0 0 00 25 ?0 ?1 ?0 ?0 ?0 ?0 ?1 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         12 ps>DECODE_12       : 1 0 00 25 ?0 ?1 ?0 ?0 ?0 ?0 ?3 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         13 ps>                : 0 0 00 25 ?0 ?1 ?0 ?0 ?0 ?0 ?3 ?0 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         14 ps>EXECUTE_14      : 1 0 00 25 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?1 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         15 ps>                : 0 0 00 25 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?1 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         16 ps>ALUWRITEBACK_16 : 1 0 00 25 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?1 ?1 ?0 ?1 ?1 ?0 1 0 ;
<         17 ps>                : 0 0 02 00 ?0 ?1 ?0 ?0 ?2 ?1 ?0 ?1 ?1 ?0 ?1 ?1 ?0 1 0 ;
[ETC]
```

Acima temos a execução de uma soma, que ativa os ciclos de estado `FETCH_2`, `DECODE_4`, `EXECUTE_6` e `ALUWRITEBACK_8`, por exemplo, realizando desde a busca dos dados nos registradores até a escrita do resultado da soma nos registradores em vários ciclos usando o menor número de componentes o possvel.
