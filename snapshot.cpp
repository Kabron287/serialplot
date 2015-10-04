/*
  Copyright © 2015 Hasan Yavuz Özderya

  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stddef.h>

#include "snapshot.h"
#include "snapshotview.h"

Snapshot::Snapshot(QMainWindow* parent, QString name) :
    QObject(parent),
    _showAction(name, this),
    _deleteAction("Delete", this)
{
    _name = name;

    view = NULL;
    mainWindow = parent;
    connect(&_showAction, &QAction::triggered, this, &Snapshot::show);

    _deleteAction.setToolTip(QString("Delete ") + _name);
    connect(&_deleteAction, &QAction::triggered, this, &Snapshot::onDeleteTriggered);
}

Snapshot::~Snapshot()
{
    if (view != NULL)
    {
        delete view;
    }
}

QAction* Snapshot::showAction()
{
    return &_showAction;
}

QAction* Snapshot::deleteAction()
{
    return &_deleteAction;
}

void Snapshot::show()
{
    if (view == NULL)
    {
        view = new SnapshotView(mainWindow, this);
        connect(view, &SnapshotView::closed, this, &Snapshot::viewClosed);
    }
    view->show();
    view->activateWindow();
    view->raise();
}

void Snapshot::viewClosed()
{
    view->deleteLater();
    view = NULL;
}

void Snapshot::onDeleteTriggered()
{
    emit deleteRequested(this);
}

QString Snapshot::name()
{
    return _name;
}

void Snapshot::setName(QString name)
{
    _name = name;
    _showAction.setText(_name);
    emit nameChanged(this);
}