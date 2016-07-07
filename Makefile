
PROJECTS=asm dasm docgen dump emulator link

all: dirs $(PROJECTS)

dirs:
	mkdir -p bin/
	mkdir -p obj/devices/

$(PROJECTS):
	make -C $@