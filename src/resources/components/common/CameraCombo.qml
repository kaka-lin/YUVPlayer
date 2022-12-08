import QtQuick 2.8
import QtQuick.Controls 2.1
//import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import CameraInfo 1.0

ComboBox {
	property string deviceName;
	property variant cameraDelegateList;

    currentIndex: 0

	model: ListModel{}
    deviceName: currentText

	function updateSPI() {
		cameraDelegateList = globalInfo.availableCameras
		model.clear()
		for (var i in cameraDelegateList) {
            model.append({
                text: cameraDelegateList[i].slice(cameraDelegateList[i].indexOf('::') + 2)
            })
		}
	}

	CameraInfo {
	    id: globalInfo
        cameraDelegate: CameraInfo.DeviceName;
        onAvailableCamerasChanged: updateSPI()
	}

    Component.onCompleted: updateSPI()
}
