/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.6
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1
import Qt.labs.folderlistmodel 1.0

Window {
    id: root
    visible: true
    height: Screen.desktopAvailableHeight;
    width: Screen.desktopAvailableWidth;
    maximumHeight: root.height;
    maximumWidth: root.width
    minimumHeight: root.height
    minimumWidth    : root.width
    color: "black"
    property int highestZ: 0
    property real defaultSize: root.width - 40
    property var currentFrame: undefined
    property real surfaceViewportRatio: 1.0
    property real swipe_distance:0.0
    property real start_coord: 0.0
    property bool changing: false
    property int nxt_fw_item_nr:0
    property int nxt_bw_item_nr:0
    property bool ready_loaded: false
    property int pic_cnt: 0
    property int pics_ges:0
    property int cur_pic:0
    property int nr:0
    property int fade_back_cnt:0
    property bool slideshow: false
    property bool jumptozero: false
    property bool jumptozerointent: false
    property bool jumptoend: true

    //UX PARAMETERS -> TO CONFIGURE Application
    property int wait_before_slide_show: 15000
    property int slide_duration: 18000

    MouseArea {
      anchors.fill: parent
      enabled: false
      cursorShape: Qt.BlankCursor

    }

    Timer {
        id:nix_time
        interval: wait_before_slide_show; running: true; repeat: true
        onTriggered: {
            //grid16.currentItem = grid16.currentItem++
            slide_time.start();
            slideshow = true;
        }
    }


    Timer {
        id:slide_time
        interval: slide_duration; running: false; repeat: true
        onTriggered: {
            if(slideshow){
                if (grid16.currentIndex < grid16.count-1){
//                    grid16.currentIndex++; //with nice transition!
                    grid16.positionViewAtIndex(grid16.currentIndex++, ListView.SnapPosition); //without transition
                }else{
                    grid16.positionViewAtBeginning();
                }
            }else{
                slide_time.stop();
            }

        }
    }

    ListView {
        id: grid16;
        x: 0; y: 0;
        cacheBuffer : 9000000
        orientation: ListView.Horizontal
        anchors.fill: parent
        //        width: 490; height: 490;
        spacing: 20
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange

        onCurrentItemChanged:  {
            console.debug(grid16.currentIndex);
        }

        onMovingChanged: { }


        onFlickStarted: {
            slideshow = false;
            nix_time.restart();
        }

//        Behavior on x {
//             NumberAnimation { duration: 100; to: grid16.currentIndex}
//         }

        FolderListModel {
            id: folderModel
            objectName: "folderModel"
            showDirs: false
            nameFilters: imageNameFilters
            onRowsInserted: {
                pics_list.clear();
                pic_cnt = 0;
                console.debug("Rows changed");
            }
            sortReversed: true
        }

        Component {
            id: fileDelegate
            Image {
                //            width: 490; height: 490; source: value;
                width: root.width;
                height: root.height;
                fillMode: Image.PreserveAspectFit;
                source: folderModel.folder + fileName;
                antialiasing: false
                autoTransform: true
                Component.onCompleted: {
                    pics_list.append({ "path": folderModel.folder + fileName});
                    pic_cnt++;
                    pics_ges = folderModel.count;
                    if (folderModel.count == pic_cnt){
                        ready_loaded = true;
                        //                            folderModel.sortField = 1;
                        //                            folderModel.sortReversed = true;
                        //                            refresh(pic_cnt);
                        console.debug("redy_loaded");
                        //                            console.debug(folderModel.count);
                        //                            image2.source = folderModel.folder + pics_list.get(0).path;
                    }

                    //                        console.debug(filePath)
                    //                        console.debug(folderModel.count)

                }
            }
        }
        model: folderModel
        delegate: fileDelegate
    }

    ListModel{
        id: pics_list
    }



    Component.onCompleted: {
        if (typeof contextInitialUrl !== 'undefined') {
            // Launched from C++ with context properties set.
            imageNameFilters = contextImageNameFilters;
            picturesLocation = contextPicturesLocation;
            folderModel.folder = picturesLocation + "/";
        } else {
            // Launched via QML viewer without context properties set.
            // fileDialog.open();
        }

    }

    Binding { target: folderModel; property: "nameFilters"; value: img_con.nameFiltersAct }

//    Connections {
//        target: downloader
//        onSuccess: {
//            folderModel.nameFilters = contextImageNameFilters;
//            console.log("The application data changed!");
//        }
//    }

    property var imageNameFilters : [];
    property string picturesLocation : "";
}
