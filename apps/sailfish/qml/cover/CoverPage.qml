/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../"

CoverBackground {
    id: cover
    property QtObject player: kodi.activePlayer
    property QtObject currentItem: player ? player.currentItem : null
    property bool hasThumbnail: cover.currentItem && cover.currentItem.thumbnail.length

    function playPause() {
        cover.player.playPause();
    }

    function stop() {
        cover.player.stop();
    }

    Image {
        source: "./background.png"
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
    }

    Image {
        id: thumbnail
        width: cover.hasThumbnail > 0 ? parent.width - 2*Theme.paddingLarge : 80

        anchors.top: parent.top
        anchors.bottom: description.top
        anchors.topMargin: Theme.paddingLarge
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        visible: cover.hasThumbnail
        source: cover.hasThumbnail > 0 ? cover.currentItem.thumbnail : ""
        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: description
        anchors.verticalCenter: cover.verticalCenter
        anchors.left: cover.left
        anchors.right: cover.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        color: Theme.primaryColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        fontSizeMode: Text.HorizontalFit
        height: lineCount * font.pixelSize
    }

    CoverActionList {
        CoverAction {
            id: leftAction
            property QtObject action
            onTriggered: action.execute()
            iconSource: icons.get(action.icon)
        }

        CoverAction {
            id: rightAction
            property QtObject action
            onTriggered: action.execute()
            iconSource: icons.get(action.icon)
        }
    }

    states: [
        State {
            when: cover.player && (cover.player.state === "playing" || cover.player.state === "paused")
            PropertyChanges {
                target: description
                text: cover.currentItem ? (cover.currentItem.title + "\n" + cover.currentItem.subtitle) : ""
            }
            PropertyChanges {
                target: leftAction
                action: actions.get(settings.getAction("coverPlayingOne", "playPause"))
            }
            PropertyChanges {
                target: rightAction
                action: actions.get(settings.getAction("coverPlayingTwo", "stop"))
            }
        },
        State {
            when: kodi.connected
            PropertyChanges {
                target: description
                text: qsTr("Kodi on") + "\n" + kodi.connectedHostName
            }
            PropertyChanges {
                target: leftAction
                action: actions.get(settings.getAction("coverConnectedOne", "musicLibrary"))
            }
            PropertyChanges {
                target: rightAction
                action: actions.get(settings.getAction("coverConnectedTwo", "videoLibrary"))
            }
        },
        State {
            when: !kodi.connected
            PropertyChanges {
                target: description
                text: qsTr("Kodimote") + "\n" +
                      qsTr("Disconnected")
            }
            PropertyChanges {
                target: leftAction
                action: actions.get(settings.getAction("coverDisconnectedOne", "addHost"))
            }
            PropertyChanges {
                target: rightAction
                action: actions.get(settings.getAction("coverDisconnectedTwo", "changeHost"))
            }
        }
    ]
}


