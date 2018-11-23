import cv2

img = cv2.imread("build/rgb.ppm")
cv2.imwrite("build/rgb.jpg", img)