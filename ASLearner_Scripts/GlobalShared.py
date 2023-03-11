from keras.models import load_model
from keras.applications import mobilenet_v2
import cv2

state_live_translation = False
state_learn_the_basics = False
state_sign_improver = False
model = load_model("DenseNet16998.06896551724138.h5")
preprocessing_object = mobilenet_v2
live_text_translated = ""
live_learning_basics_text = ""
live_sign_improver_text = ""
camera = cv2.VideoCapture(0)
learning_index = 0
sign_improver_index = 0
label_key = {0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E', 5: 'F', 6: 'G', 7: 'H', 8: 'I', 9: 'J', 10: 'K', 11: 'L',
             12: 'M', 13: 'N', 14: 'O', 15: 'P', 16: 'Q', 17: 'R', 18: 'S', 19: 'T', 20: 'U', 21: 'V', 22: 'W',
             23: 'X', 24: 'Y', 25: 'Z', 26: 'del', 27: 'nothing', 28: 'space'}
