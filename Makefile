all:
	cd src && make all && cd ..
	cd examples && make all && cd ..

clean:
	cd src && make clean && cd ..
	cd examples && make clean && cd ..
