LIB=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
PROJECTNAME=catAndMouse
SRC=src
BUILDPATH=build
CC=g++


ifeq ($(OS),Windows_NT)
	SHELL=cmd
	COPYCMD=copy
	DELCMD=del
	MOVECMD=move
	PATHSEP2=\\
	LIBLOC=C:\bin\SFML-2.4.1\lib
	INCLUDE=-I "C:\bin\SFML-2.4.1\include"
	NULLDIR=null
	BUILD=.\$(BUILDPATH)
else
    UNAME_S := $(shell uname -s)
	COPYCMD=cp
	DELCMD=rm
	MOVECMD=mv
	PATHSEP2=/
	BUILD=$(BUILDPATH)

    ifeq ($(UNAME_S),Linux)
	NULLDIR=/dev/null
    endif
    ifeq ($(UNAME_S),Darwin)

    endif
endif


OBJS= alienattack.o attack.o bigMap.o character.o chest.o damageTrap.o entity.o grid.o helper.o interactable.o network.o main.o trap.o   

all: linux

%.o: $(SRC)/%.cpp 
	@echo ==============================
	@echo Compiling $<
	$(CC) $(INCLUDE) -c $< -o $@ -std=c++11

window: wmakedir wmoveback windowlink
	
windowlink:  $(OBJS)
	@echo ==============================
	@echo Linking target $@
	$(CC) -L $(LIBLOC) -o $(PROJECTNAME).exe $(OBJS) $(LIB)
	
	@if NOT EXIST sfml-system-2.dll @echo ============================== & @echo Copying Libraries from SFML & @$(COPYCMD) $(LIBLOC)\*.dll . 
	
	@if EXIST sfml-system-d-2.dll @echo ============================== & @echo Deleting Unnecessary Libraries & @$(DELCMD) *-d-2.dll 
		
	@echo ==============================
	@echo Moving objects files to $(BUILD)
	@if EXIST main.o @move *.o $(BUILD)
	@echo ==============================
	@echo Build Success

linux: makedir moveback linuxlink

linuxlink: $(OBJS)
	@echo ==============================
	@echo Linking target $@
	$(CC) -o $(PROJECTNAME) $(OBJS) $(LIB)
	@echo ==============================
	@echo Moving objects files to $(BUILD)
	$(MOVECMD) *.o $(BUILD)
	@echo Build Success
	
moveback:
	if [ -f $(BUILD)/main.o ]; then $(MOVECMD) $(BUILD)/*.o . ; fi
	
makedir:
	if [ ! -d "$(BUILD)" ]; then mkdir $(BUILD); fi

wmoveback:
	@if EXIST .\build\main.o $(MOVECMD) $(BUILD)\*.o .
	
wmakedir:
	@if NOT EXIST .\build mkdir $(BUILD)

clean:
	@echo ==============================
	@echo Cleaning directories
	$(DELCMD) $(PROJECTNAME)* $(BUILD)$(PATHSEP2)*.o *.dll 1 *.o
	@echo Done

