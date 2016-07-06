all:
	(cd 3rdparty/gmp;./configure;make)
	(cd ..; cd ..)
	(cd src;make)
	@echo all done.

clean:
	(cd src;make clean)
	@echo clean done.

