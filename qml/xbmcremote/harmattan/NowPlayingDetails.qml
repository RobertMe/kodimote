import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Page {
    id: nowPlayingDetails
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: player.currentItem
    clip: true

    Component.onCompleted: print("currentitem is", currentItem)

    tools: ToolBarLayout {
        id: toolBarBack
        visible: false
        ToolIcon { platformIconId: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
            }
        }
    }

    Image {
        source: currentItem.thumbnail
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        opacity: .35
    }
    Flickable {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: parent.width
        contentHeight: labelColumn.height
        clip: true
        z: 5
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: labelColumn
            spacing: 5
            height: childrenRect.height
            anchors.left: parent.left
            anchors.right: parent.right


            Label {
                text: currentItem.title
                width: parent.width
                elide: Text.ElideRight
                font.pixelSize: 32
                font.bold: true
            }

            Label {
                text: currentItem.subtitle
                width: parent.width
                elide: Text.ElideRight
                visible: text.length > 0
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.rating > -1
                Label { text: qsTr("Rating:"); font.bold: true }
                Label { text: currentItem.rating }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.season > -1
                Label { text: qsTr("Season:"); font.bold: true }
                Label { text: currentItem.season }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.episode > 1
                Label { text: qsTr("Episode:"); font.bold: true }
                Label { text: currentItem.episode }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.firstAired.length > 0
                Label { text: qsTr("First aired:"); font.bold: true }
                Label { text: currentItem.firstAired }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.genre.length > 0
                Label { text: qsTr("Genre:"); font.bold: true }
                Label { text: currentItem.genre }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.year.length > 0
                Label { text: qsTr("Year:"); font.bold: true }
                Label { text: currentItem.year }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.director.length > 0
                Label { text: qsTr("Director:"); font.bold: true }
                Label { text: currentItem.director }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.tagline.length > 0
                Label { text: qsTr("Tagline:"); font.bold: true }
                Label { text: currentItem.tagline }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.mpaa.length > 0
                Label { text: qsTr("MPAA:"); font.bold: true }
                Label { text: currentItem.mpaa }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.instrument.length > 0
                Label { text: qsTr("Instrument:"); font.bold: true }
                Label { text: currentItem.instrument }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.style.length > 0
                Label { text: qsTr("Style:"); font.bold: true }
                Label { text: currentItem.style }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.mood.length > 0
                Label { text: qsTr("Mood:"); font.bold: true }
                Label { text: currentItem.mood }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.born.length > 0
                Label { text: qsTr("Born:"); font.bold: true }
                Label { text: currentItem.born }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.formed.length > 0
                Label { text: qsTr("Formed:"); font.bold: true }
                Label { text: currentItem.formed }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.died.length > 0
                Label { text: qsTr("Died:"); font.bold: true }
                Label { text: currentItem.died }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.disbanded.length > 0
                Label { text: qsTr("Disbanded:"); font.bold: true }
                Label { text: currentItem.disbanded }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.duration.length > 0
                Label { text: qsTr("Duration:"); font.bold: true }
                Label { text: currentItem.duration }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.comment.length > 0
                Label { text: qsTr("Comment:"); font.bold: true }
                Label { text: currentItem.comment }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.playcount.length > 0
                Label { text: qsTr("Playcount:"); font.bold: true }
                Label { text: currentItem.playcount }
            }

            Row {
                width: parent.width; spacing: 10; visible: currentItem.cast.length > 0
                Label { text: qsTr("Cast:"); font.bold: true }
                Label { text: currentItem.cast }
            }


            Label {
                id: plotLabel; width: parent.width; visible: currentItem.plot.length > 0
                text: currentItem.plot;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            Label {
                id: descriptionLabel; width: parent.width; visible: currentItem.description.length > 0
                text: currentItem.description;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

    }

}
