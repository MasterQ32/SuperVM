
#disassembler.o: ../common/disassembler.c
#	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^

%.o: ../common/%.c
	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^
	
#vm.o: ../common/vm.c
#	$(CC) $(FLAGS) -c -o $(addprefix $(TMPDIR), $@) $^
