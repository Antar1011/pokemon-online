#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QtCore>
#include <QtScript>

#include "client.h"

class Client;

class ScriptEngine : public QObject
{
    Q_OBJECT

public:
    ScriptEngine(Client *client);
    ~ScriptEngine();
    void loadScripts();
    void updateScripts(const QString &scripts);
    /* Exist Functions */
    bool channelExist(const QString &function, int channelId);
    bool playerExist(const QString &function, int playerId);

private:
    Client *myClient;
    QScriptEngine myEngine;
    QScriptValue myScript;

    QVector<bool> stopEvents;

    void evaluate(const QScriptValue &expression);
    void startStopEvent();
    bool endStopEvent();
    void printLine(const QString &message);
    void warn(const QString &function, const QString &message);

    template<typename ...Params>
    void MakeEvent(const QString &event, const Params&&... params);
    template<typename ...Params>
    bool MakeSEvent(const QString &event, const Params&&... params);
};

template<typename ...Params>
void ScriptEngine::MakeEvent(const QString &event, const Params&&... params) {
    if(!myScript.property(event, QScriptValue::ResolveLocal).isValid()) {
        return;
    }
    evaluate(myScript.property(event).call(myScript, QScriptValueList() << params...));
}

template<typename ...Params>
bool ScriptEngine::MakeSEvent(const QString &event, const Params&&... params) {
    if(!myScript.property(event, QScriptValue::ResolveLocal).isValid()) {
        return true;
    }
    startStopEvent();

    evaluate(myScript.property(event).call(myScript, QScriptValueList() << params...));

    return !endStopEvent();
}

#endif // SCRIPTENGINE_H
