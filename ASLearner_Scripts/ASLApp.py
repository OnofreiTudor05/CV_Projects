import cv2
from kivy.app import App
from kivy.core.image import Texture
from kivy.core.window import Window
from kivy.uix.screenmanager import ScreenManager
from kivy.uix.screenmanager import Screen
from kivy.lang import Builder
from kivy.clock import Clock
import numpy as np


class FirstWindow(Screen):
    @staticmethod
    def close_app():
        App.get_running_app().stop()


class SecondWindow(Screen):
    pass


class LiveTranslation(Screen):
    camera_texture = None
    camera_size = (360, 640, 3)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.camera = cv2.VideoCapture(0)

        Clock.schedule_interval(self.update_camera, 1.0/30.0)

    def update_camera(self, *args):
        return_value, frame = self.camera.read()
        buffer = bytes(cv2.flip(frame, -1))
        texture = Texture.create(size=(frame.shape[1], frame.shape[0]), colorfmt='bgr')

        texture.blit_buffer(buffer, colorfmt='bgr', bufferfmt='ubyte')
        camera_size = np.shape(frame)
        self.ids["camera"].texture = texture

    @staticmethod
    def close_app():
        App.get_running_app().stop()


class WindowManager(ScreenManager):
    pass


class ASLApp(App):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def build(self):
        kv = Builder.load_file('layout.kv')
        Window.size = (360, 640)
        return kv


if __name__ == "__main__":
    ASLApp().run()
