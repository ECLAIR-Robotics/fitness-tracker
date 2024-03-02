###################################################################
#                                                                 #
#                    PLOT A LIVE GRAPH (PyQt5)                    #
#                  -----------------------------                  #
#                  Requires: Python 3.6.5, PyQt5                  #
#                                                                 #
###################################################################

'''
Currently just visualizes Y_accelerometer, which is a rough approximator for cycle/period length (e.g. valleys represent end of rep)
'''

import sys
# import os
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
# import functools
import numpy as np
import random as rd
import pandas as pd
import matplotlib
from datetime import datetime, timedelta
matplotlib.use("Qt5Agg")
from matplotlib.figure import Figure
from matplotlib.animation import TimedAnimation
from matplotlib.lines import Line2D
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import time
import threading

class CustomMainWindow(QMainWindow):
    def __init__(self):
        super(CustomMainWindow, self).__init__()
        # Define the geometry of the main window
        self.setGeometry(300, 300, 800, 400)
        self.setWindowTitle("Data Visualization")
        # Create FRAME_A
        self.FRAME_A = QFrame(self)
        self.FRAME_A.setStyleSheet("QWidget { background-color: %s }" % QColor(210,210,235,255).name())
        self.LAYOUT_A = QGridLayout()
        self.FRAME_A.setLayout(self.LAYOUT_A)
        self.setCentralWidget(self.FRAME_A)
        # Place the zoom button
        self.zoomBtn = QPushButton(text = 'zoom')
        self.zoomBtn.setFixedSize(100, 50)
        self.zoomBtn.clicked.connect(self.zoomBtnAction)
        self.LAYOUT_A.addWidget(self.zoomBtn, *(0,0))
        # Place the matplotlib figure
        self.myFig = CustomFigCanvas()
        self.LAYOUT_A.addWidget(self.myFig, *(0,1))
        # Add the callbackfunc to ..
        myDataLoop = threading.Thread(name = 'myDataLoop', target = dataSendLoop, daemon = True, args = (self.addData_callbackFunc,))
        myDataLoop.start()
        self.show()
        return

    def zoomBtnAction(self):
        print("zoom in")
        self.myFig.zoomIn(5)
        return

    def addData_callbackFunc(self, value):
        # print("Add data: " + str(value))
        self.myFig.addData(value)
        return


'''
an attempt at including all data points. currently unsuccessful/unused, but would be cool as a live running visual
'''
def plot_activity(activity, data):
    fig, (ax0, ax1, ax2, ax3, ax4, ax5, ax6, ax7, ax8) = matplotlib.subplots(nrows=9,
         figsize=(30, 20),
         sharex=True)
    plot_axis(ax0, data['timestamp'], data['X_a'], 'X_Accelerometer')
    plot_axis(ax1, data['timestamp'], data['Y_a'], 'Y_Accelerometer')
    plot_axis(ax2, data['timestamp'], data['Z_a'], 'Z_Accelerometer')

    plot_axis(ax3, data['timestamp'], data['X_m'], 'X_Magnetometer')
    plot_axis(ax4, data['timestamp'], data['Y_m'], 'Y_Magnetometer')
    plot_axis(ax5, data['timestamp'], data['Z_m'], 'Z_Magnetometer')
    
    plot_axis(ax6, data['timestamp'], data['X_g'], 'X_Gyroscope')
    plot_axis(ax7, data['timestamp'], data['Y_g'], 'Y_Gyroscope')
    plot_axis(ax8, data['timestamp'], data['Z_g'], 'Z_Gyroscope')
    matplotlib.subplots_adjust(hspace=0.2)
    fig.suptitle(activity)
    matplotlib.subplots_adjust(top=0.90)
    matplotlib.show()

def plot_axis(ax, x, y, title):
    ax.plot(x, y, 'r')
    ax.set_title(title)
    ax.xaxis.set_visible(False)
    ax.set_ylim([min(y) - np.std(y), max(y) + np.std(y)])
    ax.set_xlim([min(x), max(x)])
    ax.grid(True)

def read_data(file_path):
    column_names = ['X_a','Y_a','Z_a','X_m','Y_m','Z_m','X_g','Y_g','Z_g', 'timestamp', 'grade', 'exercise']
    df = pd.read_csv(file_path,
                     header=None,
                     names=column_names)

    return df


class CustomFigCanvas(FigureCanvas, TimedAnimation):
    def __init__(self):
        self.addedData = []
        print(matplotlib.__version__)
        # The data
        self.xlim = 200
        self.n = np.linspace(0, self.xlim - 1, self.xlim)
        a = []
        b = []
        a.append(2.0)
        a.append(4.0)
        a.append(2.0)
        b.append(4.0)
        b.append(3.0)
        b.append(4.0)
        self.y = (self.n * 0.0) + 50
        # The window
        self.fig = Figure(figsize=(5,5), dpi=100)
        self.ax1 = self.fig.add_subplot(111)
        # self.ax1 settings
        self.ax1.set_xlabel('Time')
        self.ax1.set_ylabel('Y_Accelerometer')
        self.line1 = Line2D([], [], color='blue')
        self.line1_tail = Line2D([], [], color='red', linewidth=2)
        self.line1_head = Line2D([], [], color='red', marker='o', markeredgecolor='r')
        self.ax1.add_line(self.line1)
        self.ax1.add_line(self.line1_tail)
        self.ax1.add_line(self.line1_head)
        self.ax1.set_xlim(0, self.xlim - 1)
        self.ax1.set_ylim(-100, 100)

        # self.ax2 = self.fig.add_subplot(211)
        # # self.ax2 settings
        # self.ax2.set_xlabel('Time')
        # self.ax2.set_ylabel('Z_Accelerometer')
        # self.line2 = Line2D([], [], color='blue')
        # self.line2_tail = Line2D([], [], color='red', linewidth=2)
        # self.line2_head = Line2D([], [], color='red', marker='o', markeredgecolor='r')
        # self.ax2.add_line(self.line2)
        # self.ax2.add_line(self.line2_tail)
        # self.ax2.add_line(self.line2_head)
        # self.ax2.set_xlim(0, self.xlim - 1)
        # self.ax2.set_ylim(-100, 100)
        FigureCanvas.__init__(self, self.fig)
        TimedAnimation.__init__(self, self.fig, interval = 50, blit = True)
        return

    def new_frame_seq(self):
        return iter(range(self.n.size))

    def _init_draw(self):
        lines = [self.line1, self.line1_tail, self.line1_head]
        for l in lines:
            l.set_data([], [])
        return

    def addData(self, value):
        # print(value)
        self.addedData.append(value)
        return

    def zoomIn(self, value):
        bottom = self.ax1.get_ylim()[0]
        top = self.ax1.get_ylim()[1]
        bottom += value
        top -= value
        self.ax1.set_ylim(bottom,top)
        self.draw()
        return

    def _step(self, *args):
        # print(args)
        # Extends the _step() method for the TimedAnimation class.
        try:
            TimedAnimation._step(self, *args)
        except Exception as e:
            self.abc += 1
            print(str(self.abc))
            TimedAnimation._stop(self)
            pass
        return

    def _draw_frame(self, framedata):
        margin = 2
        while(len(self.addedData) > 0):
            self.y = np.roll(self.y, -1)
            self.y[-1] = self.addedData[0]
            del(self.addedData[0])

        self.line1.set_data(self.n[ 0 : self.n.size - margin ], self.y[ 0 : self.n.size - margin ])
        self.line1_tail.set_data(np.append(self.n[-10:-1 - margin], self.n[-1 - margin]), np.append(self.y[-10:-1 - margin], self.y[-1 - margin]))
        self.line1_head.set_data(self.n[-1 - margin], self.y[-1 - margin])
        self._drawn_artists = [self.line1, self.line1_tail, self.line1_head]
        return

''' End Class '''


# You need to setup a signal slot mechanism, to
# send data to your GUI in a thread-safe way.
# Believe me, if you don't do this right, things
# go very very wrong..
class Communicate(QObject):
    data_signal = pyqtSignal(float)

''' End Class '''

def dataSendLoop(addData_callbackFunc):
    # Setup the signal-slot mechanism.
    mySrc = Communicate()
    mySrc.data_signal.connect(addData_callbackFunc)

    # iterate through all of the files, one after another, and read their Y-acc
    fileNum = 1
    s = '../IMU_data/data9'+str(fileNum)+'.csv'
    df = read_data(s)
    df['timestamp'] = (df['timestamp'] - df['timestamp'][0])/1000   

    y = df['Y_a']

    # Deprecated variables
    # currFilePeak = 0
    # prevPeakTime, prevPeriod = 0, 0
    # plot_activity("Activity", subset)

    # Simulate some data
    # n = np.linspace(0, 499, 500)
    # y = 50 + 25*(np.sin(n / 8.3)) + 10*(np.sin(n / 7.5)) - 5*(np.sin(n / 1.5))

    # print(np.shape(y), int(np.shape(y)[0]), y)

    # Data is read into the list y, and the current signal value is y[i]
    while(True):
        # Handle iterating to the next file
        if(i >= int(np.shape(y)[0])):
            i = 0
            repCount = 0
            firstRep = True
            valleyValue = -200
            goingUp = True

            s = '../IMU_data/data'+str(fileNum)+'.csv'
            df = read_data(s)
            df['timestamp'] = (df['timestamp'] - df['timestamp'][0])/1000   
            y = df['Y_a']
            if fileNum > 90:
                fileNum = 0
                continue
            fileNum += 1

        time.sleep(0.1)
        mySrc.data_signal.emit(y[i]) # <- Here you emit a signal to the visual!

        # logic for counting reps (i.e. periods)
        if i != 0 and y[i] < y[i - 1] and goingUp:
            # peak
            goingUp = False
        elif i != 0 and y[i] > y[i - 1] and not goingUp:
            # valley
            if firstRep:
                firstRep = False
                valleyValue = y[i]
            # print("Valley value: ", y[i])
            goingUp = True
            if abs(y[i] - valleyValue) < 10:
                repCount += 1
                print("Rep: ", repCount)
        i += 1
        
        #     currPeakTime = datetime.now().timestamp()
        #     period = currPeakTime - prevPeakTime
        #     print("here")
        #     if abs((period - prevPeriod)) > 1 and prevPeriod != 0:
        #         # print("Slow!", period, prevPeriod, period-prevPeriod)
        #         prevPeriod = currPeakTime
        # else:
        #     goingUp = True
        #     currFilePeak = y[i]
    ###
###

if __name__== '__main__':
    app = QApplication(sys.argv)
    QApplication.setStyle(QStyleFactory.create('Plastique'))
    myGUI = CustomMainWindow()
    sys.exit(app.exec_())