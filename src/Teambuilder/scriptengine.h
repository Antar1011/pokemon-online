#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QtCore>
#include <QtScript>

class ScriptEngine : public QObject
{
    Q_OBJECT
public:
    ScriptEngine();
    ~ScriptEngine();
};

#endif // SCRIPTENGINE_H
