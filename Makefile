SRC_DOOM = main.o http.o encoding.o jsmn.o
WTK_PATH=$(realpath ../WTK2.5.2)

all:	j2sky-j2me.jad

tmpclasses/Cibyl.j: program
	cibyl-mips2java -O -Ipolyfill -d tmpclasses program

include/cibyl-syscalls.db: syscalls/sys/include/cibylmem.h Makefile
	cibyl-generate-c-header -o polyfill syscalls sys

j2sky-j2me.jad: j2sky-j2me.jad.templ j2sky-j2me.jar
	cat $< > $@
	echo -n "MIDlet-Jar-Size: " >> $@
	wc -c j2sky-j2me.jar | cut -d ' ' -f -1 >> $@

j2sky-j2me.jar: tmpclasses/Cibyl.j
	install -d res
	cp `cibyl-config --prefix`/mips-cibyl-elf/sys-root/usr/share/java/Main.java tmpclasses
	cp `cibyl-config --prefix`/mips-cibyl-elf/sys-root/usr/share/java/GameScreenCanvas.java tmpclasses
	cd tmpclasses && javac -classpath : -source 1.2 -bootclasspath $(WTK_PATH)/lib/cldcapi11.jar:$(WTK_PATH)/lib/midpapi20.jar -source 1.2 *.java
	preverify -classpath $(WTK_PATH)/lib/cldcapi11.jar:$(WTK_PATH)/lib/midpapi20.jar: -d classes/ tmpclasses/
	cp tmpclasses/program.data.bin res/
	jar cfm $@ MANIFEST.MF -C classes . -C res .

program: $(SRC_DOOM)
	mips-cibyl-elf-g++ -Os -o $@ $+ -lm -lc
%.o:	%.c include/cibyl-syscalls.db
	mips-cibyl-elf-g++ -Os -c -o $@ $< -Ipolyfill/ -fpermissive

