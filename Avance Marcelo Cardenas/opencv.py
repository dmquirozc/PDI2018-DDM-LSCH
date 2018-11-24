# IMPORTING LIBRARIES
import numpy as np
import cv2
import math

#LOADING HAND CASCADE
hand_cascade = cv2.CascadeClassifier('Hand_haar_cascade.xml')
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
agest = cv2.CascadeClassifier('fist.xml')

def on_change(val):
	return None

low_thresh = 0
high_thresh = 255

# VIDEO CAPTURE
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
winTitle = "Ventana de calibración"
tbTitle_L = "Umbral inferior"
tbTitle_H = "Umbral superior"

cv2.namedWindow(winTitle)
cv2.createTrackbar(tbTitle_L, winTitle , 0, 255, on_change)
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


	cv2.imshow('Original', img)
	cv2.imshow('Binaria', masked_hand)

	'''final = cv2.GaussianBlur(img2,(7,7),0)
	im, contours, hierarchy = cv2.findContours(final, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	cv2.drawContours(img, contours, 0, (255,255,0), 3)
	cv2.drawContours(final, contours, 0, (255,255,0), 3)



	if len(contours) > 0:
		cnt=contours[0]
		hull = cv2.convexHull(cnt, returnPoints=False)
		# finding convexity defects
		defects = cv2.convexityDefects(cnt, hull)
		count_defects = 0
		# applying Cosine Rule to find angle for all defects (between fingers)
		# with angle > 90 degrees and ignore defect
		if defects!= None:
			for i in range(defects.shape[0]):
				p,q,r,s = defects[i,0]
				finger1 = tuple(cnt[p][0])
				finger2 = tuple(cnt[q][0])
				dip = tuple(cnt[r][0])
				# find length of all sides of triangle
				a = math.sqrt((finger2[0] - finger1[0])**2 + (finger2[1] - finger1[1])**2)
				b = math.sqrt((dip[0] - finger1[0])**2 + (dip[1] - finger1[1])**2)
				c = math.sqrt((finger2[0] - dip[0])**2 + (finger2[1] - dip[1])**2)
				# apply cosine rule here
				angle = math.acos((b**2 + c**2 - a**2)/(2*b*c)) * 57.29
				# ignore angles > 90 and highlight rest with red dots
				if angle <= 90:
				    count_defects += 1
		# define actions required
		if count_defects == 1:
			cv2.putText(img,"THIS IS 2", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 2, 2)
		elif count_defects == 2:
			cv2.putText(img, "THIS IS 3", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 2, 2)
		elif count_defects == 3:
			cv2.putText(img,"This is 4", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 2, 2)
		elif count_defects == 4:
			cv2.putText(img,"THIS IS 5", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 2, 2)
    '''




	k = cv2.waitKey(30) & 0xff
	if k == 27:
		break

cap.release()
cv2.destroyAllWindows()
