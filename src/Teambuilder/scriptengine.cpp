#include "scriptengine.h"
#include "poketextedit.h"

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

void ScriptEngine::warn(const QString &function, const QString &message)
{
    printLine(QString("Script Warning in %1: %2").arg(function, message));
}

/* Test Functions */

bool ScriptEngine::channelExist(const QString &function, int channelId)
{
    if(!myClient->channelNames.contains(channelId)) {
        if(function.length() > 0) {
            warn(function, QString("No channel numbered %1 exists").arg(channelId));
        return false;
        }
    }
    return true;
}

bool ScriptEngine::playerExist(const QString &function, int playerId)
{
    if(!myClient->playerExist(playerId)) {
        if(function.length() > 0) {
            warn(function, QString("No player numbered %1 exists").arg(playerId));
        }
        return false;
    }
    return true;
}

/* Messages */

void ScriptEngine::clearChat()
{
    myClient->mychat->clear();
}

QString ScriptEngine::getAnnouncement()
{
    return myClient->announcement->toPlainText();
}

void ScriptEngine::print(QScriptContext *context, QScriptEngine *)
{
    QString result;
    for (int i = 0; i < context->argumentCount(); ++i) {
        if(i > 0) {
            result.append(" ");
        }
        result.append(context->argument(i).toString());
    }
    printLine(result);
}

void ScriptEngine::sendMessage(const QString &message, int channelId)
{
    if(channelExist("sendMessage(message, channelId)", channelId) && message.length() > 0) {
        myClient->relay().sendChanMessage(channelId, message);
    }
}

void ScriptEngine::sendPM(int playerId, const QString &message)
{
    if(!myClient->pmedPlayers.contains(playerId) && message.length() > 0) {
        myClient->startPM(playerId);
        myClient->relay().sendPM(playerId, message);
    }
    if(message.length() > 0) {
        myClient->relay().sendPM(playerId, message);
    }

}
