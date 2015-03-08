/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
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
import harbour.kodimote 1.0

Dialog {
    id: dialog

    property variant disconnectedActions: actions.list(Action.UseCaseDisconnected)
    property variant connectedActions: actions.list(Action.UseCaseConnected)
    property variant playingActions: actions.list(Action.UseCasePlaying)

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: header.height + column.height

        DialogHeader {
            id: header
            acceptText: qsTr("Save")
            width: parent.width
        }

        Column {
            id: column
            anchors {left: parent.left; right: parent.right; top: header.bottom; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
            spacing: Theme.paddingSmall

            SectionHeader {
                text: qsTr("Disconnected")
            }

            ComboBox {
                id: disconnectedOne
                label: qsTr("Left")
                currentIndex: disconnectedActions.indexOf(settings.getAction("coverDisconnectedOne", "addHost"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.disconnectedActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }

            ComboBox {
                id: disconnectedTwo
                label: qsTr("Right")
                currentIndex: disconnectedActions.indexOf(settings.getAction("coverDisconnectedTwo", "changeHost"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.disconnectedActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Connected")
            }

            ComboBox {
                id: connectedOne
                label: qsTr("Left")
                currentIndex: connectedActions.indexOf(settings.getAction("coverConnectedOne", "musicLibrary"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.connectedActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }

            ComboBox {
                id: connectedTwo
                label: qsTr("Right")
                currentIndex: connectedActions.indexOf(settings.getAction("coverConnectedTwo", "videoLibrary"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.connectedActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Playing")
            }

            ComboBox {
                id: playingOne
                label: qsTr("Left")
                currentIndex: playingActions.indexOf(settings.getAction("coverPlayingOne", "playPause"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.playingActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }

            ComboBox {
                id: playingTwo
                label: qsTr("Right")
                currentIndex: playingActions.indexOf(settings.getAction("coverPlayingTwo", "stop"))
                menu: ContextMenu {
                    Repeater {
                        model: dialog.playingActions
                        MenuItem {
                            text: title
                        }
                    }
                }
            }
        }
    }

    onAccepted: {
        settings.bindAction("coverDisconnectedOne", disconnectedActions.get(disconnectedOne.currentIndex).identifier);
        settings.bindAction("coverDisconnectedTwo", disconnectedActions.get(disconnectedTwo.currentIndex).identifier);
        settings.bindAction("coverConnectedOne", connectedActions.get(connectedOne.currentIndex).identifier);
        settings.bindAction("coverConnectedTwo", connectedActions.get(connectedTwo.currentIndex).identifier);
        settings.bindAction("coverPlayingOne", playingActions.get(playingOne.currentIndex).identifier);
        settings.bindAction("coverPlayingTwo", playingActions.get(playingTwo.currentIndex).identifier);
    }
}
