all:
	(cd 3rdparty/gmp; ./configure --enable-shared=no; make)
	(cd ..; cd ..)
	make -C src
	@echo all done.

clean:
	make -C src clean
	@echo clean done.

