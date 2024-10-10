/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>
#include <unistd.h>
#include <string>


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    std::string username = getlogin();
    std::string pathname = "/home/" + username;
    QString qpathname = QString::fromStdString(pathname);
    const QString rootPath = qpathname;

    QFileSystemModel f_model;
    f_model.setRootPath(rootPath);

    QWidget main_w;
    QVBoxLayout *layout = new QVBoxLayout(&main_w);

    QLineEdit filter;
    filter.setPlaceholderText("Filter...");

    f_model.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

    QTreeView tree;
    tree.setModel(&f_model);
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = f_model.index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid())
            tree.setRootIndex(rootIndex);
    }

    layout->addWidget(&filter);
    layout->addWidget(&tree);

    QObject::connect(&filter, &QLineEdit::textChanged,
                 [&](const QString &text) {
            f_model.setNameFilterDisables(false);
            f_model.setNameFilters(QStringList() << "*" + text + "*");
                 }
    );

    main_w.setLayout(layout);
    main_w.setWindowTitle("Files");

    QScreen *screen = app.primaryScreen();
        if (screen){
            QSize size = screen->availableGeometry().size();
            main_w.resize(size.width()/2, size.height()/1.5);
        }
        else main_w.resize(1200, 600);

    main_w.show();

    return app.exec();
}
