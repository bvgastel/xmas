#include "validplugin.h"

#include <QUrl>
#include <QColor>

QString ValidPlugin::name() {
    return m_name;
}

void ValidPlugin::name(QString name) {
    m_name = name;
}

QMap<QString, QString> ValidPlugin::parameters() {
    return m_paramMap;
}

void ValidPlugin::parameters(QMap<QString, QString> paramMap) {
    m_paramMap = paramMap;
}

bool ValidPlugin::run_json_string(QString jsonString) {
    size_t len = 10 < jsonString.length() ? 10 : jsonString.length();
    std::cout << "Started process validate network for json starting with '"
              << jsonString.toStdString().substr(0, len)
              << "'" << std::endl;
    return true;
}

bool ValidPlugin::run_json_file(QUrl fileUrl) {
    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger->log("Opening file " + filename, QColor("black"));

    std::cout << "Started process validate network for file '"
              << filename
              << std::endl;

    return true;
}

bool ValidPlugin::run_json_xmas(std::map<std::string, XMASComponent * > ) {
    std::cout << "Started process validate network for components." << std::endl;
    return true;
}


