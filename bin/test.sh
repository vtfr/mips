#!/bin/sh
echo "Executando suite de testes..."
cd work/

# Função responsável por converter os VHDLs para o formato aceitado pelo 
# Alliance
compilar()
{
	vasy -a -o $1 $1 || exit
}

# Função responsável por simular os códigos VHDL com os arquivos PAT gerados
simular() 
{
	asimut -b $1 $1 result || exit
}

# Passa o vasy nos arquivos
compilar mux2
compilar mux4
compilar alu
compilar shift2
compilar signextend
compilar reg
compilar reg_enable
compilar regfile
compilar control

echo "INICIANDO SIMULAÇÃO"
simular mux2
simular mux4
simular alu
simular shift2
simular signextend
simular reg
simular reg_enable
simular regfile
simular control

echo Circuito rodando com sucesso!

