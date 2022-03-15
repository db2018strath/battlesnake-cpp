# Constants
CXX=g++

LINKFLAGS=-lboost_system -lpthread
INCLUDEFLAGS=-Ivendor/crow/include

OTHER_FLAGS=

DEBUGFLAGS=-Wall -ggdb -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
RELEASEFLAGS=-O2
CPPFLAGS=-std=c++17

OUTNAME=battlesnake

OUTDIR=out
OUTDIR_DEBUG=$(OUTDIR)/debug
OUTDIR_RELEASE=$(OUTDIR)/release
OUT_DEBUG=$(OUTDIR_DEBUG)/$(OUTNAME)
OUT_RELEASE=$(OUTDIR_RELEASE)/$(OUTNAME)

# Obj output
objdir=objdir
debugObjDir=$(objdir)/debug
releaseObjDir=$(objdir)/release

objs=main.o

debugObjs=$(addprefix $(debugObjDir)/,$(objs))
releaseObjs=$(addprefix $(releaseObjDir)/,$(objs))

# Headers
headers=

# Debug Build
$(OUT_DEBUG): $(debugObjs)
	$(CXX) -o $@ $(debugObjs) $(CPPFLAGS) $(LINKFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

$(debugObjDir)/%.o: %.cpp $(headers) | objdirs
	$(CXX) -c -o $@ $(patsubst $(debugObjDir)/%,%,$(@:.o=.cpp)) $(INCLUDEFLAGS) $(CPPFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

# Release Build
$(OUT_RELEASE): $(releaseObjs)
	$(CXX) -o $@ $(releaseObjs) $(CPPFLAGS) $(LINKFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)

$(releaseObjDir)/%.o: %.cpp $(headers) | objdirs
	$(CXX) -c -o $@ $(patsubst $(releaseObjDir)/%,%,$(@:.o=.cpp)) $(INCLUDEFLAGS) $(CPPFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)


.PHONY: debug
debug: $(OUT_DEBUG)


.PHONY: release
release: $(OUT_RELEASE)


# Helpers
.PHONY: objdirs
objdirs:
	-mkdir $(objdir)
	-mkdir $(debugObjDir) $(releaseObjDir)
	-mkdir $(OUTDIR) $(OUTDIR_DEBUG) $(OUTDIR_RELEASE)


.PHONY: clean
clean:
	-rm $(OUT_DEBUG) $(OUT_RELEASE) $(debugObjs) $(releaseObjs)
	-rmdir $(OUTDIR_DEBUG) $(OUTDIR_RELEASE) $(OUTDIR)
	-rmdir $(debugObjDir) $(releaseObjDir)
	-rmdir $(objdir)

