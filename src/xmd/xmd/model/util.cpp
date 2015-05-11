/*********************************************************************
  *
  * Copyright (C) <name>, <year>
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#include <QTextStream>
#include <QFile>
#include <QUrl>

#include "model/util.h"

Util::Util(QObject *parent) : QObject(parent)
{

}

Util::~Util()
{

}

std::string Util::fileName(QUrl fileUrl) {

    return (fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                 : fileUrl.fileName().toStdString());
}


QString Util::modelPath() {
    return QDir::homePath() + "/xmas-models";
}

bool Util::saveFile(QUrl fileUrl, QString contents) {
    QString filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile()
                                  : fileUrl.fileName();
    // Write to file
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        return false;
    }
    QTextStream out(&file);
    out << contents;
    file.close();
    return true;
}


QString Util::saveAs(QUrl fileUrl)
{
    QFileDialog dialog(nullptr);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectoryUrl(fileUrl);
    dialog.selectFile(fileUrl.toLocalFile());

    QUrl file;
    if (dialog.exec())
        file = dialog.selectedUrls().at(0);
    else
        return fileUrl.fileName();

    return file.fileName();
}


QString Util::openFile(QUrl fileUrl) {
    QString filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile()
                                  : fileUrl.fileName();

    emit (new Util())->writeLog(Designer, "Opening file " + filename);
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return QString();
    }

    QTextStream in(&file);

    QString contents;
    in >> contents;

    return contents;
}


