import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('images/locker.jpg',0)

##img = cv2.threshold(img,127,255, cv2.THRESH_BINARY_INV)

img = cv2.resize(img, dsize=(150, 50), interpolation=cv2.INTER_AREA)

get = []
c = 0
for a in img:
    for b in a:
        if b > 100: c=0
        else: c=1
        get.append(c)

get_str = str(get)
get_str = get_str.replace("[", "{")
get_str = get_str.replace("]","};")

print(get_str)
