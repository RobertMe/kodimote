/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
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
import "../components/"

Page {
    id: mainPage

    property bool connected: xbmc.connected

    states: [
        State {
            when: connected
            PropertyChanges { target: listView.headerItem; visible: true }
            PropertyChanges { target: mainMenu; visible: true; enabled: true }
            PropertyChanges { target: listView; model: mainMenuModel }
            PropertyChanges { target: noConnection; visible: false }
        }
    ]

    onConnectedChanged: {
        pageStack.pop(mainPage);
        populateMainMenu();
        if (connected) {
            pageStack.pushAttached("XbmcPage.qml");
        } else {
            pageStack.popAttached();
        }
    }

    function showConnect(operationType) {
        pageStack.push("ConnectionDialog.qml", undefined, operationType);
    }

    function browse(target) {
        var menuModel = null;

        for (var i = mainMenuModel.count; i--;) {
            menuModel = mainMenuModel.get(i);
            if (menuModel.target === target) {
                break;
            }
        }

        if (menuModel < 0) {
            return;
        }

        var newModel;
        if (menuModel.mode === "library") {
            newModel = xbmc[menuModel.libraryTarget]();
        } else {
                newModel = xbmc.shares(menuModel.target);
        }

        console.log("setting model: " + newModel);
        var browser = pageStack.push("BrowserPage.qml", {model: newModel});
        browser.home.connect(function() {
            pageStack.pop(mainPage);
        });
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: null
        property int currentSelected

        header: PageHeader {
            id: pageHeader
            visible: false
            title: qsTr("Browse %1").arg(xbmc.connectedHostName)
        }

        PullDownMenu {
            id: mainMenu
            visible: false
            enabled: false
            ControlsMenuItem {

            }

            MenuItem {
                text: qsTr("Keypad")
                onClicked: {
                    pageStack.push("Keypad.qml")
                }
            }

            MenuItem {
                text: qsTr("Now playing")
                enabled: xbmc.activePlayer !== null
                onClicked: {
                    pageStack.push("NowPlayingPage.qml")
                }
            }
        }

        PushUpMenu {
            MenuItem {
                text: qsTr("Change connection...")
                onClicked: showConnect()
            }

            MenuItem {
                id: settingsMenu
                text: qsTr("Settings")
                onClicked: {
                    pageStack.push("SettingsDialog.qml");
                }
            }

            MenuItem {
                text: qsTr("About")
                onClicked: {
                    pageStack.push("AboutDialog.qml");
                }
            }
        }

        NoConnection {
            id: noConnection
            anchors.fill: parent
            visible: true

            onShowConnect: mainPage.showConnect()
        }

        delegate: ListItem {
            id: listItem

            contentHeight: Theme.itemSizeExtraLarge
            menu: contextMenuComponent

            Image {
                id: img
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter

                source: icon
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: img.right
                anchors.right: parent.right
                anchors.leftMargin: 14

                Label {
                    id: mainText
                    text: listView.model.title(index)
                    font.weight: Font.Bold
                    font.pixelSize: Theme.fontSizeLarge
                }

                Label {
                    id: subText
                    text: {
                        if (mode === "library") {
                            return qsTr("Library");
                        } else if (mode == "files") {
                            return qsTr("Files");
                        } else {
                            return ""
                        }
                    }
                    visible: libraryTarget && sharesTarget
                    color: Theme.secondaryColor
                }
            }

            onPressed: listView.currentSelected = index;

            showMenuOnPressAndHold: hasMenu

            onClicked: browse(target)

            Component {
                id: contextMenuComponent

                ContextMenu {
                    id: contextMenu

                    MenuItem {
                        visible: mode !== "files" && mode !== "single"
                        text: qsTr("Show files")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "files";
                            settings[target + "ShowsFiles"] = true;
                        }
                    }
                    MenuItem {
                        visible: mode !== "library" && mode !== "single"
                        text: qsTr("Show library")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "library";
                            settings[target + "ShowsFiles"] = false;
                        }
                    }
                    MenuItem {
                        text: qsTr("Rescan library")
                        onClicked: {
                            var lib = xbmc[libraryTarget]();
                            lib.scanForContent();
                            lib.exit();
                        }
                    }
                    MenuItem {
                        text: qsTr("Clean library")
                        onClicked: {
                            var lib = xbmc[libraryTarget]();
                            lib.clean();
                            lib.exit();
                        }
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (!xbmc.connected && !xbmc.connecting) {
                showConnect();
            }

            if (xbmc.connected) {
                pageStack.pushAttached("XbmcPage.qml");
            }
        }
    }

    Component.onCompleted: {
        populateMainMenu();
    }

    ListModel {
        id: mainMenuModelTemplate
        ListElement {
            icon: "image://theme/icon-l-music"
            subtitle: ""
            mode: "library"
            target: "music"
            libraryTarget: "audioLibrary"
            sharesTarget: "music"
            hasMenu: true
        }
        ListElement {
            icon: "image://theme/icon-l-video"
            subtitle: ""
            mode: "library"
            target: "video"
            libraryTarget: "videoLibrary"
            sharesTarget: "video"
            hasMenu: true
        }
        ListElement {
            icon: "image://theme/icon-l-image"
            subtitle: ""
            mode: "files"
            target: "pictures"
            sharesTarget: "picture"
            hasMenu: false
        }
        ListElement {
            icon: "../icons/icon-l-tv.png"
            subtitle: ""
            mode: "library"
            libraryTarget: "pvrMenu"
            target: "tv"
        }
    }
    ListModel {
        id: mainMenuModel
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            var item = mainMenuModel.get(index);

            if (item) {
                var target = mainMenuModel.get(index).target;
                if (target === "music") {
                    return qsTr("Music");
                }
                if (target === "video") {
                    return qsTr("Videos");
                }
                if (target === "pictures") {
                    return qsTr("Pictures");
                }
                if (target === "tv") {
                    return qsTr("Live TV");
                }
            }
            return "";
        }
    }

    function populateMainMenu() {
        mainMenuModel.clear();
        if (settings.musicEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(0));
        }
        if (settings.videosEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(1));
        }
        if (settings.picturesEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(2));
        }
        if (settings.pvrEnabled && xbmc.pvrAvailable) {
            mainMenuModel.append(mainMenuModelTemplate.get(3));
        }

        if(settings.musicShowsFiles) {
           mainMenuModel.setProperty(0, "mode", "files");
        }
        if(settings.videoShowsFiles) {
            mainMenuModel.setProperty(1, "mode", "files");
        }
    }

    Connections {
        target: settings
        onMusicEnabledChanged: populateMainMenu();
        onVideosEnabledChanged: populateMainMenu();
        onPicturesEnabledChanged: populateMainMenu();
        onPvrEnabledChanged: populateMainMenu();
    }

    Connections {
        target: xbmc
        onPvrAvailableChanged: populateMainMenu();
    }
}
