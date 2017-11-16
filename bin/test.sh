#!/bin/sh
echo "Executando suite de testes..."
cd work/

# Passa o vasy nos arquivos
   vasy -a -o inv_4bits{,} \
&& vasy -a -o acc_4bits{,} \
&& vasy -a -o mux_4bits{,} \
&& vasy -a -o somador_1bit{,} \
&& vasy -a -o somador_4bits{,} \
&& vasy -a -o somador_4bits_acc{,}

# Passa o asimut nos arquivos
   asimut -b inv_4bits{,} result \
&& asimut -b acc_4bits{,} result \
&& asimut -b mux_4bits{,} result \
&& asimut -b somador_1bit{,} result \
&& MBK_CATAL_NAME=catalog MBK_IN_LO=vst asimut somador_4bits{,} result \
&& MBK_CATAL_NAME=catalog MBK_IN_LO=vst asimut somador_4bits_acc{,} result \
&& echo Circuito rodando com sucesso!

