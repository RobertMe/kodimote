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

Item {
    id: libraryView
    anchors.fill: parent

    signal goBack

    property alias library: list.model
    property alias view: list

    function incrementCurrentIndex() {
        list.incrementCurrentIndex();
    }
    function decrementCurrentIndex() {
        list.decrementCurrentIndex();
    }
    function selectItem(index) {
        if(list.model.get(index, "filetype") ==="directory") {
            list.enterItem(index);
            list.currentIndex = 0;
        } else {
            list.model.playItem(index);
        }
    }
    function goUp() {
        if(library.parentModel() !== null) {
            library = library.exit()
        } else {
            libraryView.goBack()
        }
    }
    function showContextMenu() {
        // Hack: if we clear the model, the anchors are screwed for some reason. To prevent this
        // we unset the model and set it back after populating it
        contextMenu.model = null
        contextMenuModel.clear();
        if(library.get(list.currentIndex, "playable")) {
            contextMenuModel.append({ "entryId": 0 })
            contextMenuModel.append({ "entryId": 1 })
        }
        if(kodiBrowser.viewState === "library" && list.model.parentModel() === null) {
            contextMenuModel.append({ "entryId": 2 })
        }
        if(library.hasDetails()) {
            contextMenuModel.append({ "entryId": 3 });
        }
        if(list.model.parentModel() !== null) {
            contextMenuModel.append( {"entryId": 4 });
        }

        if(contextMenuModel.count > 0) {
            contextMenu.open();
        }
        contextMenu.model = contextMenuModel
    }

    BrowsingTopBar {
        id: topBar
        anchors.left: parent.left
        anchors.top: parent.top
        currentDir: library.title
        currentIcon: library.parentModel() !== null ? "images/HomeBack.png" : "images/HomeIcon.png"

//        onGoUp: {
//            if(library.parentModel() !== null) {
//                library = library.exit()
//            }
//        }
        onGoBack: {
            if(library.parentModel() !== null) {
                library = library.exit()
            } else {
                libraryView.goBack()
            }
        }
    }

    BorderImage {
        id: listBackground
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15
        anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: topBar.bottom }
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
//        state: library.state
        source: "images/ContentPanel.png"

        ListView {
            id: list
            anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: parent.top}
            anchors.margins: 25
            //        anchors.rightMargin: 20
            clip: true
            model: library
            currentIndex: 0

            function enterItem(index) {
                var newModel = list.model.enterItem(index);
                newModel.ignoreArticle = settings.ignoreArticle;
                console.log("newModel: " + newModel);
                list.model = newModel;
            }
            function playItem(index) {
                list.model.playItem(index);
            }

            delegate: Item {
                width: parent.width
                height: 64
                Image {
                    anchors.fill: parent
                    source: index === list.currentIndex ? "images/MenuItemFO.png" : "images/MenuItemNF.png"
                }

                Image {
                    id: thumbnailImage
                    height: parent.height - 2
                    anchors.top: parent.top
                    anchors.topMargin: 1
    //                width: height
                    fillMode: Image.PreserveAspectFit
                    smooth: false
                    source: settings.useThumbnails ? thumbnail : ""
                    sourceSize.height: parent.height - 2
                    visible: settings.useThumbnails
                }


                Text {
                    color: "white"
                    text: title
                    font.pixelSize: 28
                    anchors {left: parent.left; top: parent.top; bottom: parent.bottom; right: watchedImage.left }
                    anchors.leftMargin: thumbnailImage.width > 0 ? thumbnailImage.width + 10 : 0
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                Image {
                    id: watchedImage
                    source: "images/OverlayWatched.png"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    visible: playcount > 0
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: contextMenu.state != "visible"

                    onPressed: list.currentIndex = index

                    onPressAndHold: {
                        showContextMenu();
                    }

                    onClicked: {
                        if(filetype === "directory") {
                            list.enterItem(index);
                        } else {
                            list.model.playItem(index);
                        }
                    }
                }
            }

            Keys.onPressed: {
                console.log("key pressed");
            }
        }

        ScrollBar {
            scrollArea: list; width: 15; alwaysVisible: false
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom}
            anchors.rightMargin: 10
            anchors.topMargin: 20
            anchors.bottomMargin: 20
        }


    }

    ListModel {
        id: contextMenuModel
        ListElement { entryId: 0 }
        ListElement { entryId: 1 }
        ListElement { entryId: 2 }
        ListElement { entryId: 3 }
        ListElement { entryId: 4 }
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            if (title["text"] === undefined) {
                title.text = [
                    qsTr("Play"),
                    qsTr("Add to playlist"),
                    qsTr("Rescan library"),
                    qsTr("Details"),
                    qsTr("Download")
                ]
            }
            return title.text[index];
        }

    }

    ContextMenu {
        id: contextMenu

        model: contextMenuModel

        onAccepted: {
            switch(index) {
            case 0:
                list.model.playItem(list.currentIndex);
                break;
            case 1:
                list.model.addToPlaylist(list.currentIndex);
                break;
            case 2:
                list.model.scanForContent();
                break;
            case 3:
                itemDetails.forceActiveFocus();
                break;
            case 4:
                list.model.download(list.currentIndex, "/home/user/MyDocs/");
                break;
            }

            if(index !== 3) {
                mainFocusArea.forceActiveFocus();
            } else {
                list.model.fetchItemDetails(list.currentIndex);
                itemDetails.item = list.model.getItem(list.currentIndex);
                print("******index is: " + list.currentIndex)
                print("******list.model: " + list.model);
                print("******list.model: " + list.model.getItem(list.currentIndex));
                print("******item set to: " + itemDetails.item);
            }
        }

        onRejected: {
            mainFocusArea.forceActiveFocus();
        }
    }



//    Behavior on opacity {
//        NumberAnimation { easing.type: Easing.InQuint; duration: 300 }
//    }

    states: [
        State {
            name: "hidden"; when: !visible
            PropertyChanges { target: listBackground; opacity: 0}
            PropertyChanges { target: topBar; opacity: 0; anchors.leftMargin: -topBar.width}
        },
        State {
            name: "visible"; when: visible
            PropertyChanges { target: listBackground; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.InQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  300; easing.type: Easing.OutQuad}
        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.OutQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  200; easing.type: Easing.InQuad}
            NumberAnimation { target: topBar; property: "opacity"; duration: 300; easing.type: Easing.OutQuad}
        }
    ]

    ItemDetails {
        id: itemDetails
        anchors.fill: parent
//        opacity: 0

        onClosed: mainFocusArea.forceActiveFocus();
    }

}
