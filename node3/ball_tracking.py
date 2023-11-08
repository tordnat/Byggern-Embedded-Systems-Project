import time
import imutils
import cv2
import argparse
import numpy as np

from collections import deque
from imutils.video import VideoStream

arg_parser = argparse.ArgumentParser()

arg_parser.add_argument("-v")
