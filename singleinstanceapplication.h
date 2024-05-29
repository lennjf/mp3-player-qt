#ifndef SINGLEINSTANCEAPPLICATION_H
#define SINGLEINSTANCEAPPLICATION_H
#include <QApplication>
class SingleInstanceApplication : public QApplication
{
public:
    SingleInstanceApplication(int &argc, char **argv);
    bool createInstance(int &argc, char **argv);
};

#endif // SINGLEINSTANCEAPPLICATION_H
