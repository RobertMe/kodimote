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

import Qt 4.7

Rectangle {
    id: screen
    color: "black"

    property string orientation:  width > height ? "landscape" : "portrait"


    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Component.onCompleted: homeMenu.forceActiveFocus();

    Image  {
        id: backgroundImageMusic
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "backgrounds/music.jpg"
        opacity:  kodiBrowser.mediaState == "audio" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

    Image {
        id: backgroundImageVideos
        source: "backgrounds/videos.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity:  kodiBrowser.mediaState == "video" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

    Image {
        id: backgroundImagePictures
        source: "backgrounds/pictures.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity:  kodiBrowser.mediaState == "pictures" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

    FocusScope {
        id: mainFocusArea
        anchors.fill: parent
        focus: true
        property string inputText
        Timer {
            id: inputTimer
            interval: 1500
            repeat: false
            triggeredOnStart: false

            onTriggered: parent.inputText = ""
        }

        Keys.onPressed: {
            if(event.modifiers === Qt.ControlModifier) {
                if(event.key === Qt.Key_Right){
                    view.incrementCurrentIndex();
                }
            } else if(event.key === Qt.Key_Up) {
                itemModel.currentView.decrementCurrentIndex();
            } else if(event.key === Qt.Key_Down) {
                itemModel.currentView.incrementCurrentIndex();
            } else if(event.key === Qt.Key_Right || event.key === Qt.Key_Return) {
                itemModel.currentView.selectItem(itemModel.currentView.view.currentIndex);
            } else if(event.key === Qt.Key_Left || event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
                itemModel.currentView.goUp();
            } else if(event.key === Qt.Key_Space) {
                itemModel.currentView.showContextMenu();
            } else if(event.key === Qt.Key_AltGr) {
                // eat the FN key to prevent jumping to index 0
            }else {
                inputTimer.stop();
                inputText += event.text;
                itemModel.currentView.view.currentIndex = itemModel.currentView.library.findItem(inputText);
                itemModel.currentView.view.positionViewAtIndex(itemModel.currentView.library.findItem(inputText), ListView.Beginning)
                inputTimer.start();
            }
        }
    }

    VisualItemModel {
        id: itemModel

        property QtObject currentView
//        Item {
//            property string label: "Library"
//            width: view.width; height: view.height
//            LibraryView {
//                id: libraryView
//                visible: true
//            }

//            Component.onDestruction: print("destroyed 1")
//        }

        Item {
            id: kodiBrowser
            width: view.width; height: view.height

            property string mediaState: "audio"
            property string viewState: "library"

            LibraryView {
                id: musicBrowser
                library: kodi.shares("music")
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: musicLibraryView
                library: kodi.audioLibrary()
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: videoBrowser
                library: kodi.shares("video")
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: videoLibraryView
                library: kodi.videoLibrary()
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: pictureBrowser
                library: kodi.shares("pictures")
                visible: false
                onGoBack: homeMenu.state = "open"
            }

            states: [
                State { name: "none"; when: homeMenu.state == "open" || homeMenu.state == "halfopen"
                    PropertyChanges {target:  musicBrowser; visible: false }
                    PropertyChanges {target:  musicLibraryView; visible: false }
                    PropertyChanges {target:  videoBrowser; visible: false }
                    PropertyChanges {target:  videoLibraryView; visible: false }
                    PropertyChanges {target: itemModel; currentView: null }
                },
                State { name: "audiofiles"; when: kodiBrowser.mediaState == "audio" && kodiBrowser.viewState == "files"
                    PropertyChanges {target:  musicBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: musicBrowser }
                },
                State { name: "audiolibrary"; when: kodiBrowser.mediaState == "audio" && kodiBrowser.viewState == "library"
                    PropertyChanges {target:  musicLibraryView; visible: true }
                    PropertyChanges {target: itemModel; currentView: musicLibraryView }
                },
                State { name: "videofiles"; when: kodiBrowser.mediaState == "video" && kodiBrowser.viewState == "files"
                    PropertyChanges {target:  videoBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: videoBrowser }
                },
                State { name: "videolibrary"; when: kodiBrowser.mediaState == "video" && kodiBrowser.viewState == "library"
                    PropertyChanges {target:  videoLibraryView; visible: true }
                    PropertyChanges {target: itemModel; currentView: videoLibraryView }
                },
                State { name: "pictures"; when: kodiBrowser.mediaState == "pictures" && kodiBrowser.viewState == "files"
                    PropertyChanges {target:  pictureBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: pictureBrowser }
                }

            ]

        }

        Item {
            property string label: "Now Playing"
            width: view.width
            height: view.height
            NowPlaying {
                id: nowPlaying
                anchors.fill: parent
                playlist: kodi.activePlayer.playlist()
                state: "nowPlaying"
                Keys.onPressed: {
                    if(event.modifiers === Qt.ControlModifier && event.key === Qt.Key_Left) {
                        view.decrementCurrentIndex();
                    }
                    if(event.modifiers === Qt.ControlModifier && event.key === Qt.Key_Right) {
                        view.incrementCurrentIndex();
                    }
                    event.accepted = true;
                }
            }
        }

        Item {
            width: view.width; height: view.height

            FocusScope {
                id: keypad
                Keys.onPressed: {
                    if(event.modifiers === Qt.ControlModifier) {
                        if(event.key === Qt.Key_Left) {
                            view.decrementCurrentIndex();
                        } else if(event.key === Qt.Key_Right) {
                            view.incrementCurrentIndex();
                        }
                    } else {
                        if(kodi.picturePlayerActive) {
                            pictureControls.keyPressed(event);
                        } else {
                            keyPadInternal.keyPressed(event);
                        }
                    }
                    event.accepted = true;
                }
            }

            Keypad {
                id: keyPadInternal
                visible: !kodi.picturePlayerActive
            }
            PictureControls {
                id: pictureControls
                visible: kodi.picturePlayerActive
            }

        }

    }

    ListView {
        id: view
        anchors.fill: parent
        model: itemModel
        preferredHighlightBegin: 0; preferredHighlightEnd: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem; flickDeceleration: 300
        highlightMoveSpeed: 1200
        cacheBuffer: 200

        onCurrentIndexChanged: {
            switch(currentIndex) {
            case 0:
                mainFocusArea.forceActiveFocus();
                break;
            case 1:
                nowPlaying.forceActiveFocus();
                break;
            case 2:
                keypad.forceActiveFocus();
                break;
            }

        }
    }

    Rectangle {
        id: notConnectedPopup
        anchors.centerIn: parent
        height: 250
        width: 450
        color: "black"
        border.color: "white"
        radius: 10
        opacity: 0
        scale: 0.5

        Image {
            anchors.fill: parent
            source: "images/GlassOverlay.png"
        }

        Text {
            anchors.fill: parent
            anchors.margins: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: kodi.connectionError
            wrapMode: Text.WordWrap
            color: "white"
            font.pixelSize: 28
        }
    }


    MouseArea {
        id: cancelArea
        anchors.fill: screen
        enabled: false
        onClicked: {
            homeMenu.state = "closed"
            homeMenu.subMenuState = "closed"
        }
        states: State {
            name: "homeMenuOpen"; when: homeMenu.state == "open" || homeMenu.subMenuState == "open"
            PropertyChanges { target: cancelArea; enabled: true }
        }
    }
    HomeMenu {
        id: homeMenu
        state: "open"
        onStateChanged: {
            if(state == "open") {
                homeMenu.forceActiveFocus();
            } else {
                mainFocusArea.forceActiveFocus();
            }
        }
    }

    states: [
    State {
            name: "notConnected"; when: !kodi.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0.8; scale: 1 }
            PropertyChanges { target: view; opacity: 0; }
            PropertyChanges { target: homeMenu; opacity: 0 }
        },
        State {
            name: "connected"; when: kodi.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0; scale: 0.5 }
            PropertyChanges { target: view; opacity: 1; }

            PropertyChanges {
                target: backgroundImageMusic
                x: 0
                y: 0
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "notConnected"
            NumberAnimation {properties: "opacity"; duration:  300; easing.type: Easing.InCirc}
            NumberAnimation {properties: "scale"; duration:  500; easing.type: Easing.OutBack}
        },
        Transition {
            from: "notConnected"
            to: "*"
            NumberAnimation {properties: "opacity"; duration:  200; easing.type: Easing.OutCirc}
            NumberAnimation {properties: "scale"; duration:  300; easing.type: Easing.Linear}
        }
    ]


}
