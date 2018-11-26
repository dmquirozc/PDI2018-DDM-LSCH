# IMPORTING LIBRARIES
import numpy as np
import cv2
import math

#LOADING HAND CASCADE
hand_cascade = cv2.CascadeClassifier('Hand_haar_cascade.xml')
fist_cascade = cv2.CascadeClassifier('aGest.xml')

def on_change(val):
	return None

# VIDEO CAPTURE
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
winTitle = "Ventana de calibración"
tbTitle_L = "Umbral inferior"
tbTitle_H = "Umbral superior"
x_offsetT = "Eje x"
y_offsetT = "Eje y"
w_offsetT = "Ancho"
h_offsetT = "Alto"


cv2.namedWindow(winTitle)
cv2.createTrackbar(tbTitle_L, winTitle , 35, 255, on_change)
cv2.createTrackbar(tbTitle_H, winTitle, 255, 255, on_change)
cv2.createTrackbar(x_offsetT, winTitle, 35, 100, on_change)
cv2.createTrackbar(y_offsetT, winTitle, 31, 100, on_change)
cv2.createTrackbar(w_offsetT, winTitle, 26, 100, on_change)
cv2.createTrackbar(h_offsetT, winTitle, 35, 100, on_change)

while 1:
	__, img = cap.read()
	blur = cv2.GaussianBlur(img,(5,5),0) # BLURRING IMAGE TO SMOOTHEN EDGES
	hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV) # BGR -> GRAY CONVERSION
	gray = cv2.cvtColor(blur, cv2.COLOR_BGR2GRAY)

	low_thresh = cv2.getTrackbarPos(tbTitle_L, winTitle)
	high_thresh = cv2.getTrackbarPos(tbTitle_H, winTitle)
	x_offset = cv2.getTrackbarPos(x_offsetT, winTitle)
	y_offset = cv2.getTrackbarPos(y_offsetT, winTitle)
	w_offset = cv2.getTrackbarPos(w_offsetT, winTitle)
	h_offset = cv2.getTrackbarPos(h_offsetT, winTitle)

	__, bin = cv2.threshold(hsv, low_thresh, high_thresh, cv2.ADAPTIVE_THRESH_MEAN_C) # THRESHOLDING IMAGE

	h, s, v = cv2.split(bin) # Se usa el canal de saturación ya que es inmune a la luminosidad

	kernel = np.ones((3,3),np.uint8)

	eroded = cv2.erode(s, kernel, 1)

	hand = hand_cascade.detectMultiScale(bin, 1.1, 5) # DETECTING HAND IN THE THRESHOLDE IMAGE
	fist = fist_cascade.detectMultiScale(gray, 1.1, 5)

	binary = eroded.copy()

	height, width, channels = img.shape
	canvas = np.zeros((height, width, 1), np.uint8)

	for (x,y,w,h) in hand: # MARKING THE DETECTED ROI
		cv2.rectangle(canvas, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, -1)
		cv2.rectangle(binary, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, 1)
		cv2.rectangle(img, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, 1)

	for (x,y,w,h) in fist: # MARKING THE DETECTED ROI
		cv2.rectangle(canvas, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, -1)
		cv2.rectangle(binary, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, 1)
		cv2.rectangle(img, (x + x_offset-50, y + y_offset-50), (x+w+w_offset,y+h+h_offset), 255, 1)

	masked_hand = cv2.bitwise_and(canvas, eroded)

	final = cv2.GaussianBlur(masked_hand,(7,7),0)
	im, contours, hierarchy = cv2.findContours(final, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

	hand_trace = np.zeros((height, width, 1), np.uint8)
	max_area = 0
	max_ind = 0

	for i in range(len(contours)):
		cnt = contours[i]
		area = cv2.contourArea(cnt)
		if(area > max_area):
			max_area = area
			max_ind = i

	if(len(contours) > 0):

		hand_cnt = contours[max_ind]

		cv2.drawContours(hand_trace, [hand_cnt], -1, (255,255,255), 3)
		convex_hull = cv2.convexHull(hand_cnt)
		cv2.drawContours(hand_trace, [convex_hull], -1, (255, 255, 0), 3)

		hull2 = cv2.convexHull(hand_cnt, returnPoints = False)
		defects = cv2.convexityDefects(hand_cnt, hull2)

		FarDefect = []

		for i in range(defects.shape[0]):
			s,e,f,d = defects[i,0]
			start = tuple(hand_cnt[s][0])
			end = tuple(hand_cnt[e][0])
			far = tuple(hand_cnt[f][0])
			FarDefect.append(far)
			cv2.line(hand_trace,start,end,[0,255,0],1)
			cv2.circle(hand_trace,far,10,[100,255,255],3)
	

	cv2.imshow('Original', img)
	cv2.imshow('Binaria', binary)
	cv2.imshow('Contorno + Convexo', hand_trace)

	k = cv2.waitKey(30) & 0xff
	if k == 27:
		break

cap.release()
cv2.destroyAllWindows()
