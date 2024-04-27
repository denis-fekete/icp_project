all:
	cd src; \
	qmake icp_project.pro; \
	make; \
	cd ..; \
	mv src/icp_project icp_project;
	mkdir -p build;
	mv src/*.o build/;

doxygen:
	doxygen Doxyfile

clean:
	rm -f -r doc/html
	rm -f -r build
	rm -f src/moc_*
	rm -f ui_*
	rm -f src/Makefile
	rm -f icp_project

make run:
	./icp_project
	
pack:
	zip -r xfeket01.zip src examples doc/main_page.md Makefile
