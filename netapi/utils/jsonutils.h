#ifndef JSONUTILS_H
#define JSONUTILS_H

#include "../netapinamespace.h"

#include <QObject>

NETAPI_NAMESPACE_BEGIN
class JsonUtils
{
public:
    static QByteArray transferList2Json(const QString &cmd, int cmdType, QList<QVariantMap> datas);
    static bool isJson(const QByteArray &json);
    static QString getCmd(const QByteArray &json);
    static int getCmdType(const QByteArray &json);
    static QList<QVariantMap> transferJson2List(const QByteArray &json);

public:
    static const QString JSON_CMD;
    static const QString JSON_CMD_TYPE;
    static const QString JSON_DATAS;
};
NETAPI_NAMESPACE_END
#endif // JSONUTILS_H
