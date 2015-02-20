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
                label: qsTr("Left")
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
                label: qsTr("Right")
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
                label: qsTr("Left")
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
                label: qsTr("Right")
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
                label: qsTr("Left")
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
                label: qsTr("Right")
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
}
