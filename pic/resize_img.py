from PIL import Image
import os
import PIL
import glob

image = Image.open("/media/storage/pic/103APPLE/IMG_3333.JPG")
print(image.size)
resized_image = image.resize((int(image.size[0]/8),int(image.size[1]/8)))

print(resized_image.size)
resized_image.save("/home/leon/BottleIdentifier/pic/classifier.jpg")
