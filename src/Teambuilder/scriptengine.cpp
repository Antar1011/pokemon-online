#include "scriptengine.h"

// printLine() is just a qDebug() as for now. We'll have a console or something later.

ScriptEngine::ScriptEngine(Client *client)
{
    setParent(client);
    myClient = client;

    QScriptValue sys = myEngine.newQObject(this);
    myEngine.globalObject().setProperty("sys", sys);

    loadScripts();
}

ScriptEngine::~ScriptEngine()
{

}

void ScriptEngine::loadScripts()
{
    QFile file("client-scripts.js");
    file.open(QIODevice::ReadOnly);

    updateScripts(QString::fromUtf8(file.readAll()));
}

void ScriptEngine::updateScripts(const QString &scripts)
{
    myScript = myEngine.evaluate(scripts);
    myEngine.globalObject().setProperty("script", myScript);

    if(myScript.isError()) {
        qDebug() << "Fatal Script Error Line " + QString::number(myEngine.uncaughtExceptionLineNumber()) + ": " + myScript.toString();
    } else {
        qDebug() << "Script Check: OK";
    }
}

void ScriptEngine::evaluate(const QScriptValue &expression)
{
    if(expression.isError()) {
        printLine(QString("Script Error in line %1: %2").arg(myEngine.uncaughtExceptionLineNumber()).arg(expression.toString()));
    }
}

void ScriptEngine::startStopEvent() {
    stopEvents.push_back(false);
}

bool ScriptEngine::endStopEvent() {
    bool res = stopEvents.back();
    stopEvents.pop_back();
    return res;
}

void ScriptEngine::printLine(const QString &message)
{
    qDebug() << message;
}
