#include "singleinstanceapplication.h"
#include<QMutex>

SingleInstanceApplication::SingleInstanceApplication(int &argc, char **argv) : QApplication(argc, argv) {}

bool SingleInstanceApplication::createInstance(int &argc, char **argv) {
    // Check if another instance is already running
    QMutex mutex("SingleInstanceApplication");
    QSharedMemory sharedMemory(name());

    if (!sharedMemory.attach()) {
        // No other instance is running, create a new one
        if (!sharedMemory.create(1)) {
            return false;
        }

        return QApplication::createInstance(argc, argv);
    }

    // Another instance is running, activate the existing one
    QWindowList windows = QApplication::topLevelWidgets();
    if (!windows.isEmpty()) {
        windows[0]->activateWindow();
    }

    return false;
}
