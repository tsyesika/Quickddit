/*
    Quickddit - Reddit client for mobile phones
    Copyright (C) 2020  Daniel Kutka

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see [http://www.gnu.org/licenses/].
*/

import QtQuick 2.9
import QtQuick.Controls 2.2
import quickddit.Core 1.0

Page {
    title: "Settings"

    ScrollView {
        contentWidth: width
        anchors.fill: parent

        Column{
            anchors {left: parent.left;right: parent.right}

            SwitchDelegate {
                width: parent.width
                text: "Compact images in feed"
                checked: persistantSettings.compactImages
                onCheckedChanged: persistantSettings.compactImages = checked
            }

            SwitchDelegate {
                width: parent.width
                text: "Full resolution images"
                checked: persistantSettings.fullResolutionImages
                onCheckedChanged: persistantSettings.fullResolutionImages = checked
                //enabled: false
            }

            ItemDelegate {
                width: parent.width
                Label {
                    id:sliderLabel
                    anchors {left: parent.left;verticalCenter: parent.verticalCenter; margins: 10}
                    text: "Thumbnail scale: ["+Math.floor(slider.value*100)+"%]"
                }
                Slider {
                    id:slider
                    anchors { right: parent.right; left:sliderLabel.right;  verticalCenter: parent.verticalCenter; margins:10 }
                    from: 0.5
                    to: 1.5
                    stepSize: 0.1
                    snapMode: "SnapAlways"
                    value: persistantSettings.scale
                    live: true
                    onMoved: persistantSettings.scale = value
                }
            }

            ItemDelegate {
                width: parent.width
                Label {
                    anchors {left: parent.left;verticalCenter: parent.verticalCenter; margins: 10}
                    text: "Preferred Video Size"
                }
                ComboBox {
                    anchors { right: parent.right;  verticalCenter: parent.verticalCenter; margins:10 }
                    model: ["360p","720p"]
                    currentIndex: appSettings.preferredVideoSize
                    onCurrentIndexChanged: {
                        appSettings.preferredVideoSize = currentIndex
                    }
                }
            }

            SwitchDelegate {
                width: parent.width
                text: "Open links internaly"
                checked: persistantSettings.linksInternaly
                onCheckedChanged: persistantSettings.linksInternaly = checked
                //enabled: false
            }

            SwitchDelegate {
                width: parent.width
                text: "ToolBar on bottom"
                checked: persistantSettings.toolbarOnBottom
                onCheckedChanged: persistantSettings.toolbarOnBottom = checked
            }

            ItemDelegate {
                width: parent.width
                text: "Accounts "
                onClicked: pageStack.push(Qt.resolvedUrl("AccountsPage.qml"));
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    width: 24
                    height: 24
                    source: "../Icons/next.svg"
                }
            }
        }
    }
}
