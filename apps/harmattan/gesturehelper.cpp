/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "gesturehelper.h"

#include <QDebug>

GestureHelper::GestureHelper(QObject *parent) :
    QObject(parent),
    m_limit(100),
    m_innerLimit(160),
    m_areaState(AreaCenter)
{
}

GestureHelper::AreaState GestureHelper::currentState()
{
    return m_areaState;
}

int GestureHelper::startX()
{
    return m_startX;
}

int GestureHelper::startY()
{
    return m_startY;
}

void GestureHelper::setStartX(int x)
{
    m_areaState = AreaCenter; // I have no idea why, but initializing in the ctor doesn not work... WTF???
    m_startX = x;
    emit startXChanged();
}

void GestureHelper::setStartY(int y)
{
    m_startY = y;
    emit startYChanged();
}

int GestureHelper::realX()
{
    return m_realX;
}

int GestureHelper::realY()
{
    return m_realY;
}

void GestureHelper::setRealX(int x)
{
    m_realX = x;

    qDebug() << "x" << x << "state" << m_areaState;
    switch(m_areaState) {
    case AreaCenter:
        if(x > m_innerLimit) {
            qDebug() << "right square entered";
            m_areaState = AreaRight;
            emit currentStateChanged();
        } else if( x < 0) {
            qDebug() << "left square entered";
            m_areaState = AreaLeft;
            emit currentStateChanged();
        }
        break;
    case AreaLeft:
        if(x > 0) {
            qDebug() << "center entered";
            m_areaState = AreaCenter;
            emit currentStateChanged();
        }
        break;
    case AreaRight:
        if(x < m_innerLimit) {
            qDebug() << "center entered";
            m_areaState = AreaCenter;
            emit currentStateChanged();
        }
        break;
    }

    emit realXChanged();
    emit depthChanged();
}

void GestureHelper::setRealY(int y)
{
    m_realY = y;

    switch(m_areaState) {
    case AreaCenter:
        if(y < 0) {
            qDebug() << "top entered";
            m_areaState = AreaTop;
            emit currentStateChanged();
        } else if(y > m_innerLimit) {
            qDebug() << "bottom entered";
            m_areaState = AreaBottom;
            emit currentStateChanged();
        }
        break;
    case AreaTop:
        if(y > 0) {
            qDebug() << "center entered";
            m_areaState = AreaCenter;
            emit currentStateChanged();
        }
        break;
    case AreaBottom:
        if(y < m_innerLimit) {
            qDebug() << "center entered";
            m_areaState = AreaCenter;
            emit currentStateChanged();
        }
        break;
    }

    emit realYChanged();
    emit depthChanged();
}

int GestureHelper::restrictedX()
{
    switch(m_areaState) {
    case AreaLeft:
        if(m_realX < -m_limit) {
            return -m_limit;
        }
        break;
    case AreaRight:
        if(m_realX > m_innerLimit + m_limit) {
            return m_innerLimit + m_limit;
        }
        break;
    case AreaTop:
    case AreaBottom:
        if(m_realX < 0) {
            return 0;
        }
        if(m_realX > m_innerLimit) {
            return m_innerLimit;
        }
        break;
    }
    return m_realX;
}

int GestureHelper::restrictedY()
{
    switch(m_areaState) {
    case AreaLeft:
    case AreaRight:
        if(m_realY < 0) {
            return 0;
        }
        if(m_realY > m_innerLimit) {
            return m_innerLimit;
        }
        break;
    case AreaTop:
        if(m_realY < -m_limit) {
            return -m_limit;
        }
        break;
    case AreaBottom:
        if(m_realY > m_innerLimit + m_limit) {
            return m_innerLimit + m_limit;
        }
        break;
    }
    return m_realY;
}

int GestureHelper::depth()
{
    switch(m_areaState) {
    case AreaLeft:
        return -m_realX;
    case AreaRight:
        return m_realX - m_innerLimit;
    case AreaTop:
        return -m_realY;
    case AreaBottom:
        return m_realY - m_innerLimit;
    }
    return 0;
}

void GestureHelper::reset()
{
    m_areaState = AreaCenter;
    emit currentStateChanged();
}
