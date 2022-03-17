# Constants
CXX=clang++

LINKFLAGS=-lboost_system -lpthread
INCLUDEFLAGS=-Ivendor/crow/include

OTHER_FLAGS=

DEBUGFLAGS=-Wall -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
RELEASEFLAGS=-O2
CPPFLAGS=-std=c++17

OUTNAME_SERVER=server
OUTNAME_AI_RUN=ai_run

OUTDIR=out
OUTDIR_DEBUG=$(OUTDIR)/debug
OUTDIR_RELEASE=$(OUTDIR)/release

OUT_SERVER_DEBUG=$(OUTDIR_DEBUG)/$(OUTNAME_SERVER)
OUT_SERVER_RELEASE=$(OUTDIR_RELEASE)/$(OUTNAME_SERVER)

OUT_AI_RUN_DEBUG=$(OUTDIR_DEBUG)/$(OUTNAME_AI_RUN)
OUT_AI_RUN_RELEASE=$(OUTDIR_RELEASE)/$(OUTNAME_AI_RUN)

# Obj output
objdir=objdir
debugObjDir=$(objdir)/debug
releaseObjDir=$(objdir)/release

objs=ai.o server_logic.o simulator.o

server_objs=$(objs) server.o
ai_run_objs=$(objs) ai_run.o


serverDebugObjs=$(addprefix $(debugObjDir)/,$(server_objs))
serverReleaseObjs=$(addprefix $(releaseObjDir)/,$(server_objs))

aiRunDebugObjs=$(addprefix $(debugObjDir)/,$(ai_run_objs))
aiRunReleaseObjs=$(addprefix $(releaseObjDir)/,$(ai_run_objs))

# Headers
headers=server_logic.hpp simulator.hpp grid.hpp

# Debug Builds
$(OUT_SERVER_DEBUG): $(serverDebugObjs)
	$(CXX) -o $@ $(serverDebugObjs) $(CPPFLAGS) $(LINKFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

$(OUT_AI_RUN_DEBUG): $(aiRunDebugObjs)
	$(CXX) -o $@ $(aiRunDebugObjs) $(CPPFLAGS) $(LINKFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

$(debugObjDir)/%.o: %.cpp $(headers) | objdirs
	$(CXX) -c -o $@ $(patsubst $(debugObjDir)/%,%,$(@:.o=.cpp)) $(INCLUDEFLAGS) $(CPPFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

# Release Builds
$(OUT_SERVER_RELEASE): $(serverReleaseObjs)
	$(CXX) -o $@ $(serverReleaseObjs) $(CPPFLAGS) $(LINKFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)

$(OUT_AI_RUN_RELEASE): $(aiRunReleaseObjs)
	$(CXX) -o $@ $(aiRunReleaseObjs) $(CPPFLAGS) $(LINKFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)

$(releaseObjDir)/%.o: %.cpp $(headers) | objdirs
	$(CXX) -c -o $@ $(patsubst $(releaseObjDir)/%,%,$(@:.o=.cpp)) $(INCLUDEFLAGS) $(CPPFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)


.PHONY: server_debug
server_debug: $(OUT_SERVER_DEBUG)

.PHONY: ai_run_debug
ai_run_debug: $(OUT_AI_RUN_DEBUG)

.PHONY: server_release
server_release: $(OUT_SERVER_RELEASE)

.PHONY: ai_run_release
ai_run_release: $(OUT_AI_RUN_RELEASE)

# Helpers
.PHONY: objdirs
objdirs:
	-mkdir $(objdir)
	-mkdir $(debugObjDir) $(releaseObjDir)
	-mkdir $(OUTDIR) $(OUTDIR_DEBUG) $(OUTDIR_RELEASE)


.PHONY: clean
clean:
	-rm $(OUT_SERVER_DEBUG) $(OUT_SERVER_RELEASE) $(OUT_AI_RUN_DEBUG) $(OUT_AI_RUN_RELEASE) $(serverDebugObjs) $(serverReleaseObjs) $(aiRunDebugObjs) $(aiRunReleaseObjs)
	-rmdir $(OUTDIR_DEBUG) $(OUTDIR_RELEASE) $(OUTDIR)
	-rmdir $(debugObjDir) $(releaseObjDir)
	-rmdir $(objdir)

