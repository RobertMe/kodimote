/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "keys.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "eventclient.h"

Keys::Keys(QObject *parent) :
    QObject(parent)
{
}

// Lets use the eventclient for left/right/up/down because
// Json doesn't work to skip during playback
void Keys::left()
{
    Xbmc::instance()->eventClient()->sendKeypress("left");
//    XbmcConnection::sendCommand("Input.Left");
}

void Keys::right()
{
    Xbmc::instance()->eventClient()->sendKeypress("right");
//    XbmcConnection::sendCommand("Input.Right");
}

void Keys::up()
{
    Xbmc::instance()->eventClient()->sendKeypress("up");
//    XbmcConnection::sendCommand("Input.Up");
}

void Keys::down()
{
    Xbmc::instance()->eventClient()->sendKeypress("down");
//    XbmcConnection::sendCommand("Input.Down");
}

void Keys::back()
{
    XbmcConnection::sendCommand("Input.Back");
}

void Keys::osd()
{
    XbmcConnection::sendCommand("Input.ShowOSD");
}

void Keys::contextMenu()
{
    XbmcConnection::sendCommand("Input.ContextMenu");
}

void Keys::info()
{
    XbmcConnection::sendCommand("Input.Info");
}

void Keys::fullscreen()
{
    executeAction("fullscreen");
}

void Keys::backspace()
{
    executeAction("backspace");
}

void Keys::red()
{
//    executeAction("red");
    Xbmc::instance()->eventClient()->sendIrPress("red");
}

void Keys::green()
{
//    executeAction("green");
    Xbmc::instance()->eventClient()->sendIrPress("green");
}

void Keys::yellow()
{
//    executeAction("yellow");
    Xbmc::instance()->eventClient()->sendIrPress("yellow");
}

void Keys::blue()
{
//    executeAction("blue");
    Xbmc::instance()->eventClient()->sendIrPress("blue");
}

void Keys::home()
{
    XbmcConnection::sendCommand("Input.Home");
}

void Keys::select()
{
    XbmcConnection::sendCommand("Input.Select");
}

void Keys::executeAction(const QString &action)
{
    QVariantMap params;
    params.insert("action", action);
    XbmcConnection::sendCommand("Input.ExecuteAction", params);
}
