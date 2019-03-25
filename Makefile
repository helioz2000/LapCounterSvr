TARGET = lcread
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

install:
#	install -o root $(TARGET) $(BINDIR)$(TARGET)
#	install -o root $(CFGFILE) $(CFGDIR)$(CFGFILE)
#	install -m 755 -o root $(INITFILE) $(INITDIR)$(INITFILE)
#	mv $(INITDIR)$(INITFILE) $(INITDIR)$(TARGET)
#	@echo ++++++++++++++++++++++++++++++++++++++++++++
#	@echo ++ Files have been installed
#	@echo ++ If you want to start $(TARGET) on boot
#	@echo ++ run the following command:
#	@echo ++ sudo update-rc.d $(TARGET) defaults
