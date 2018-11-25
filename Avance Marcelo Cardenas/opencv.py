# IMPORTING LIBRARIES
import numpy as np
import cv2
import math

#LOADING HAND CASCADE
hand_cascade = cv2.CascadeClassifier('Hand_haar_cascade.xml')

def on_change(val):
	return None

low_thresh = 36
high_thresh = 255

# VIDEO CAPTURE
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
winTitle = "Ventana de calibración"
tbTitle_L = "Umbral inferior"
tbTitle_H = "Umbral superior"

cv2.namedWindow(winTitle)
cv2.createTrackbar(tbTitle_L, winTitle , 35, 255, on_change)
cv2.createTrackbar(tbTitle_H, winTitle, 255, 255, on_change)

while 1:
	__, img = cap.read()
	blur = cv2.GaussianBlur(img,(5,5),0) # BLURRING IMAGE TO SMOOTHEN EDGES
	gray = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV) # BGR -> GRAY CONVERSION

	low_thresh = cv2.getTrackbarPos(tbTitle_L, winTitle)
	high_thresh = cv2.getTrackbarPos(tbTitle_H, winTitle)

	__, bin = cv2.threshold(gray, low_thresh, high_thresh, cv2.ADAPTIVE_THRESH_MEAN_C) # THRESHOLDING IMAGE

	h, s, v = cv2.split(bin) # Se usa el canal de saturación ya que es inmune a la luminosidad

	kernel = np.ones((3,3),np.uint8)

	eroded = cv2.erode(s, kernel, 1)
	hand = hand_cascade.detectMultiScale(s, 1.3, 5) # DETECTING HAND IN THE THRESHOLDE IMAGE

	height, width, channels = img.shape
	canvas = np.zeros((height, width, 1), np.uint8)

	for (x,y,w,h) in hand: # MARKING THE DETECTED ROI
		cv2.rectangle(canvas, (x-50,y),(x+w+50,y+h+100),255,-1)

	masked_hand = cv2.bitwise_and(canvas, eroded)

	final = cv2.GaussianBlur(masked_hand,(7,7),0)
	im, contours, hierarchy = cv2.findContours(final, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

	hand_trace = np.zeros((height, width, 1), np.uint8)
	max_area = 0
	ci = 0

	for i in range(len(contours)):
		cnt = contours[i]
		area = cv2.contourArea(cnt)
		if(area > max_area):
			max_area = area
			ci = i

	if(len(contours) > 0):

		cv2.drawContours(hand_trace, contours, ci, (255,255,255), 3)
		convex_hull = cv2.convexHull(contours[ci])
		cv2.drawContours(hand_trace, [convex_hull], -1, (255, 255, 0), 3)

	cv2.imshow('Original', img)
	cv2.imshow('Contorno + Convexo', hand_trace)

	k = cv2.waitKey(30) & 0xff
	if k == 27:
		break

cap.release()
cv2.destroyAllWindows()
