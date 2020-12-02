#include "jsonutils.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QDebug>

NETAPI_NAMESPACE_BEGIN

const QString JsonUtils::JSON_CMD = "cmd";
const QString JsonUtils::JSON_CMD_TYPE = "cmdType";
const QString JsonUtils::JSON_DATAS = "datas";


QByteArray JsonUtils::transferList2Json(const QString &cmd, int cmdType, QList<QVariantMap> datas)
{
    QJsonObject obj;

    QJsonArray jsonArray;
    for (QVariantMap data : datas) {
        jsonArray.append(QJsonObject::fromVariantMap(data));
    }

    //保证有数据，再转
    if (!jsonArray.isEmpty()) {

        obj.insert(JSON_CMD, cmd);
        obj.insert(JSON_CMD_TYPE, cmdType);
        obj.insert(JSON_DATAS, jsonArray);

        QJsonDocument doc;
        doc.setObject(obj);
        return doc.toJson(QJsonDocument::Compact);
    }

    return NULL;
}

bool JsonUtils::isJson(const QByteArray &json)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (!doc.isNull() && error.error == QJsonParseError::NoError) {
        return true;
    } else {
        qDebug() << error.errorString();
        return false;
    }
}



QString JsonUtils::getCmd(const QByteArray &json)
{
    QString cmd = "";
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (!doc.isNull() && error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            cmd = doc.object().value(JSON_CMD).toString();
        }
    }
    return cmd;
}

int JsonUtils::getCmdType(const QByteArray &json)
{
    int cmdType = 0;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (!doc.isNull() && error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            cmdType = doc.object().value(JSON_CMD_TYPE).toInt();
        }
    }
    return cmdType;
}

QList<QVariantMap> JsonUtils::transferJson2List(const QByteArray &json)
{
    QList<QVariantMap> results;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (!doc.isNull() && error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            QJsonValue jsonValue = doc.object().value(JSON_DATAS);
            if (jsonValue.isArray()) {
                QJsonArray jsonArray = jsonValue.toArray();
                for (QJsonValue value : jsonArray) {
                    if (value.isObject()) {
                        results.append(value.toObject().toVariantMap());
                    }
                }
            }
        }
    }
    return results;
}
NETAPI_NAMESPACE_END
