CXX       := g++
TARGET    := tsnd151

SRCDIR    := src
BUILDDIR  := obj
TARGETDIR := bin

SRCEXT    := cpp
DEPEXT    := d
OBJEXT    := o

CXXFLAGS  := -std=c++14 -w -g
INC       := -Isrc -Iinclude

sources      := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
objects      := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(subst $(SRCEXT),$(OBJEXT),$(sources)))
dependencies := $(subst .$(OBJEXT),.$(DEPEXT),$(objects))

all: directories $(TARGETDIR)/$(TARGET)

remake: cleaner all

run:
	./$(TARGETDIR)/$(TARGET)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

clean:
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -rf $(BUILDDIR)	

cleaner: clean
	@$(RM) -rf $(TARGETDIR)

-include $(dependencies)

$(TARGETDIR)/$(TARGET): $(objects)
	$(CXX) -o $(TARGETDIR)/$(TARGET) $^ $(LDFLAGS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	@$(CXX) $(CXXFLAGS) $(INC) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

.PHONY: all remake clean cleanerCXX  = g++

