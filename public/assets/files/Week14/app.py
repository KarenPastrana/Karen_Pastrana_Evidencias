from interfaz import *
import serial
 
class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, **kwargs):
        QtWidgets.QMainWindow.__init__(self, *args, **kwargs)
        self.setupUi(self)
        # A partir de aqui ya va lo tuyo (tu logica)

        self.serial = serial.Serial('COM14', 9600)

        self.x=0
        self.changeButton.clicked.connect(self.encender_led)
        self.Clock.clicked.connect(self.apagar_led)

        
    def encender_led(self):
        self.serial.write(b'1')
        print("LED ON")
    def apagar_led(self):
        self.serial.write(b'0')
        print("LED OFF")
 
if __name__ == '__main__':
    app = QtWidgets.QApplication([])
    window = MainWindow()
    window.show()
    app.exec()