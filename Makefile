all:
	cd src; \
	qmake icp_project.pro "CONFIG+=release"; \
	make; \
	cd ..; \
	mv src/icp_project icp_project;
	mkdir -p build;
	mv src/*.o build/;

debug:
	cd src; \
	qmake icp_project.pro "CONFIG+=debug"; \
	make; \
	cd ..; \
	mv src/icp_project icp_project;
	mkdir -p build;
	mv src/*.o build/;

time_log:
	cd src; \
	qmake icp_project.pro "CONFIG+=release" "DEFINES += LOG_TIME"; \
	make; \
	cd ..; \
	mv src/icp_project icp_project;
	mkdir -p build;
	mv src/*.o build/;

doxygen:
	doxygen Doxyfile

clean:
	rm -f -r doc/html
	rm -f -r build*
	rm -f src/moc_*
	rm -f src/.qmake.stash
	rm -f ui_*
	rm -f src/Makefile
	rm -f icp_project
	rm -f xfeket01.zip
	rm -f src/*.pro.user
	rm -f src/ui_*.h
	rm -f src/*.o


make run: all
	./icp_project
	
pack: clean
	zip -r xfeket01-xxxxxx00.zip src examples doc/* Makefile README.txt Doxyfile
