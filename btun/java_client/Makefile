#
# Making the VNC applet.
#

CP = cp
JC = javac
JCFLAGS = -target 1.1 -source 1.2
JAR = jar
ARCHIVE = BTunViewer.jar
MANIFEST = MANIFEST.MF
INSTALL_DIR = /usr/local/vnc/classes

SOURCES = $(wildcard btun/*.java) $(wildcard vnc/*.java) $(wildcard crypto/*.java) $(wildcard viewer/*.java) $(wildcard shell/*.java)
CLASSES=$(SOURCES:.java=.class)

all: $(CLASSES) $(ARCHIVE)

$(CLASSES): $(SOURCES)
	$(JC) $(JCFLAGS) -O $(SOURCES)

$(ARCHIVE): $(CLASSES) $(MANIFEST)
	$(JAR) cfm $(ARCHIVE) $(MANIFEST) $(CLASSES) `ls */*\$*.class`

install: $(CLASSES) $(ARCHIVE)
	$(CP) $(CLASSES) $(ARCHIVE) $(PAGES) $(INSTALL_DIR)

export:: $(CLASSES) $(ARCHIVE) $(PAGES)
	@$(ExportJavaClasses)

sign: jarkey $(ARCHIVE)
	jarsigner -keystore jarkey -signedjar s$(ARCHIVE) $(ARCHIVE) ngs

jarkey:
	keytool -genkey -alias ngs -keystore jarkey

clean::
	$(RM) *.class *.jar
