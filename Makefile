ALLIANCE_INCLUDE_PATH=/usr/lib/alliance/include/
ALLIANCE_LIB_PATH=/usr/lib/alliance/lib/
ALLIANCE_LIBS=-lPgn -lPat -lMut -lRcn

# Nomes dos arquivos c, sem diretório
C_FILENAMES=$(notdir $(wildcard src/c/*.c))
# Nome dos arquivos pat, sem diretório
PAT_FILENAMES=$(C_FILENAMES:.c=.pat)

# Todos os arquivo VHDL
VHDL_FILENAMES=$(notdir $(basename $(wildcard src/vhdl/*.vhdl)))

# All
all: test

# Limpa os arquivos residuais
clear:
	rm -f test/*.pat work/catalog work/*.{vbe,vhdl,vst,pat}

# Test roda a suite de testes
test: work
	./bin/test.sh

# Gera o ambiente de trabalho
work: clear genpat
	cp etc/catalog work \
	&& cp src/vhdl/*.vhdl work \
	&& cp test/*.pat work

# Compila os genpats
genpat: $(PAT_FILENAMES)

# Cria os arquivos pat com base nos arquivos C
%.pat: src/c/%.c
	$(CC) -std=c99 -o test/$(notdir $(basename $<)) $< \
		-I$(ALLIANCE_INCLUDE_PATH) \
		-L$(ALLIANCE_LIB_PATH) \
		-Iinclude/c \
		$(ALLIANCE_LIBS) \
	&& cd test \
	&& LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(ALLIANCE_LIB_PATH) \
		./$(notdir $(basename $<)) \
	&& rm $(notdir $(basename $<))
