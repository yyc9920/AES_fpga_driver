import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('images/locker.jpg',0)

img = cv2.resize(img, dsize=(0, 0), fx=0.15, fy=0.15, interpolation=cv2.INTER_AREA)

get = []
c = 0
for a in img:
    for b in a:
        if b > 100: c=1
        else: c=0
        get.append(c)

get_str = str(get)
get_str = get_str.replace("[", "{")
get_str = get_str.replace("]","};")

print(get_str)
