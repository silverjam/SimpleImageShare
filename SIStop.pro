TEMPLATE = subdirs

SUBDIRS += \
    SISLibrary \
    SISServerLibrary \
    SISLibraryTest \
    SISServer \
    SISClient \
    SIS

SISLibraryTest.depends += \
    SISLibrary \
    SISServerLibrary \

SISServerLibrary.depends += \
    SISLibrary \

SISServer.depends += \
    SISLibraryTest \
    SISLibrary \
    SISServerLibrary \

SISClient.depends += \
    SISLibraryTest \
    SISLibrary \
    SISServerLibrary \

SIS.depends += \
    SISClient \
    SISLibraryTest \
    SISLibrary \
    SISClient \
    SISServerLibrary \
