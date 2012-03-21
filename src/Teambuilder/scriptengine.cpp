#include "scriptengine.h"

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
