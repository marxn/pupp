all:
	(cd 3rdparty/gmp; ./configure --enable-shared=no; make)
	(cd ..; cd ..)
	(cd src;make)
	@echo all done.

clean:
	(cd src;make clean)
	@echo clean done.

