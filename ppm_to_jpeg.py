import cv2
import sys

mode = ""
if len(sys.argv) == 2:
    mode = str(sys.argv[1])

path = "build" + mode
img = cv2.imread(path + "/rgb.ppm")
cv2.imwrite(path + "/rgb.jpg", img)
