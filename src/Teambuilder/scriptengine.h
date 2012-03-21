#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QtCore>
#include <QtScript>

#include "client.h"

class Client;

class ScriptEngine : public QObject
{
    Q_OBJECT
    friend class Client;

public:
    ScriptEngine(Client *client);
    ~ScriptEngine();
    void loadScripts();
    void updateScripts(const QString &scripts);

private:
    Client *myClient;
    QScriptEngine myEngine;
    QScriptValue myScript;
};

#endif // SCRIPTENGINE_H
