ROOTDIR = $(PWD)

include makefiles/platforms.mk

LIBDIRS = zlibrary zldictionary
APPDIR = fbreader

all:
	@for dir in $(LIBDIRS) $(APPDIR); do \
		cd $$dir; \
		if ! $(MAKE); then \
			exit 1; \
		fi; \
		cd $(ROOTDIR); \
	done;

install: all
	cd $(APPDIR); make install

package: all
	cd $(APPDIR); make package

clean:
	@for dir in $(LIBDIRS) $(APPDIR); do \
		cd $$dir; make clean; cd $(ROOTDIR); \
	done

distclean: clean
	rm -vf *.tgz *.ipk *.deb *.prc *.log

archives: clean
