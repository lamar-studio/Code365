BIN=rw_bios_nv_user
all:$(BIN)
CFLAGS =  -g

rw_bios_nv_user:rw_bios_nv_user.o

	rm -rf config.bin
	cc -o $@ $+

clean:
	@echo CLEAN
	@rm -rf *.o $(BIN)
	@rm -rf config.bin
