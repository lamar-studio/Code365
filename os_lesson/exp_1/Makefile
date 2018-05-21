
TARGET  ?= exp

#####  shell工具  #####
RM         ?= rm -f
CLEAN_OK    = @echo "=== clean ok =====";
BUILD_SUCCESS = @echo "=== build success ==="


#####  Makefile编译规则  #####
all: $(TARGET)
	$(BUILD_SUCCESS)


$(TARGET):
	gcc -o $@ main.c -lrt 

clean:
	$(RM) $(OBJ) $(TARGET)
	$(CLEAN_OK)






