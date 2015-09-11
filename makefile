OUTDIR=out
BINDIR=bin

all: $(OUTDIR) $(BINDIR)
	$(MAKE) -C src

debug: export CXXFLAGS+=-g
debug: clean all

$(BINDIR): $(OUTDIR)
ifeq ($(OS),Windows_NT)
	xcopy /D /Y $(BINDIR)\\*.dll $(OUTDIR)
endif

$(OUTDIR):
	mkdir $(OUTDIR)

clean:
ifeq ($(OS),Windows_NT)
	del /S /Q *.o *.a *.exe $(OUTDIR)\*
	rmdir /S /Q $(OUTDIR)
else ifeq ($(shell uname), Linux)
	find . -type f -name '*.o' -exec rm -f -r -v {} \;
	find . -type f -name '*.a' -exec rm -f -r -v {} \;
	rm $(OUTDIR)/* -f
	find . -empty -type d -delete
endif

rebuild: clean all
