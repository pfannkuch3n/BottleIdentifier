from PIL import Image
import os
import PIL
import glob

image = Image.open("/media/storage/pic/103APPLE/IMG_3337.JPG")
print(image.size)
resized_image = image.resize((int(image.size[0]/16),int(image.size[1]/16)))

print(resized_image.size)
resized_image.save("/home/leon/BottleIdentifier/pic/height.jpg")
