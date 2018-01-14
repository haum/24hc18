QT += 3dcore 3drender 3dinput 3dquick 3dlogic qml quick 3dquickextras

SOURCES += \
    gamecomm.cpp \
    main.cpp \
    pheromone.cpp \
    nest.cpp \
    ant.cpp \
    food.cpp \
    gameentity.cpp

HEADERS += \
    gamecomm.h \
    pheromone.h \
    nest.h \
    ant.h \
    food.h \
    gameentity.h

OTHER_FILES += \
    main.qml \
    Scene.qml \
    Rtfm.qml \
