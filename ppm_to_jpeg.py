from PIL import Image

im = Image.open("build/rgb.ppm")
im.save("build/rgb.jpg")