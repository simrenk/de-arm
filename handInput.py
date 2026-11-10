import cv2
import mediapipe as mp
from time import time
import time

from mediapipe.tasks import python
from mediapipe.tasks.python import vision
from mediapipe.tasks.python.vision.hand_landmarker import HandLandmark
from pythonosc.udp_client import SimpleUDPClient


model_path = "/Users/simrenkshetrapal/Code/de-arm/hand_landmarker.task"
wekinator = SimpleUDPClient("127.0.0.1", 6448)

BaseOptions = mp.tasks.BaseOptions
vision = mp.tasks.vision
HandLandmarker = vision.HandLandmarker
HandLandmarkerOptions = vision.HandLandmarkerOptions
HandLandmarkerResult = vision.HandLandmarkerResult
VisionRunningMode = vision.RunningMode
drawing_utils = vision.drawing_utils
drawing_styles = vision.drawing_styles
HAND_CONNECTIONS = vision.HandLandmarksConnections.HAND_CONNECTIONS
latest_result = None



def on_result(result, output_image, timestamp_ms):
  # print(f"{timestamp_ms}: {len(result.hand_landmarks)} hand(s)")
  global latest_result
  latest_result = result

  if result.hand_landmarks:
      for hand_landmark in result.hand_landmarks:
          normalized_hands = normalize(hand_landmark)
          print(f"Number of inputs: {len(normalized_hands)}")
          wekinator.send_message("/wek/inputs", normalized_hands)
  else:
      wekinator.send_message("/wek/inputs", [0] * 126)  # Send zeros if no hands detected

def normalize(hand_landmark):
  wrist = hand_landmark[HandLandmark.WRIST]
  knuckle = hand_landmark[HandLandmark.MIDDLE_FINGER_MCP]
  palm = ((knuckle.x - wrist.x) ** 2 + (knuckle.y - wrist.y) ** 2) ** 0.5
  normalized_hands = []

  for position_name in HandLandmark:
    point = hand_landmark[position_name]
    normalized_hands += [(point.x - wrist.x) / palm, 
                          (point.y - wrist.y) / palm, 
                          (point.z - wrist.z) / palm]
  normalized_hands += [0.0] * (126 - len(normalized_hands))  # Pad with zeros if less than 42 points 
  return normalized_hands
   
options = HandLandmarkerOptions(
    base_options=BaseOptions(model_asset_path=model_path),
    running_mode=VisionRunningMode.LIVE_STREAM,
    num_hands=2,
    result_callback=on_result)

cam = cv2.VideoCapture(0)

with HandLandmarker.create_from_options(options) as landmarker:  
   while cam.isOpened():
    success, image = cam.read()
    if not success:
      # print("Ignoring empty camera frame.")
      break
    ts = time.monotonic_ns() // 1_000_000
    rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)    
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)
    hand_landmarker_result = landmarker.detect_async(mp_image, ts)

    if latest_result and latest_result.hand_landmarks:
      for hand in latest_result.hand_landmarks:
          drawing_utils.draw_landmarks(
              image,
              hand,
              HAND_CONNECTIONS,
              drawing_styles.get_default_hand_landmarks_style(),
              drawing_styles.get_default_hand_connections_style(),
          )

    cv2.imshow('MediaPipe Hands', cv2.flip(image, 1))
    if cv2.waitKey(5) & 0xFF == 27:
      break
