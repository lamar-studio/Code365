
TARGET  ?= exp

#####  shell����  #####
RM         ?= rm -f
CLEAN_OK    = @echo "=== clean ok =====";
BUILD_SUCCESS = @echo "=== build success ==="


#####  Makefile�������  #####
all: $(TARGET)
	$(BUILD_SUCCESS)


$(TARGET):
	gcc -o $@ main.c -lrt 

clean:
	$(RM) $(OBJ) $(TARGET)
	$(CLEAN_OK)






