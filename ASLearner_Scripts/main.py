import cv2
import random
import GlobalShared
import numpy as np

from time import perf_counter
from kivy.app import App
from kivy.clock import Clock
from kivy.core.image import Texture
from kivy.core.window import Window
from kivy.lang import Builder
from kivy.uix.screenmanager import Screen
from kivy.uix.screenmanager import ScreenManager
from kivy.animation import Animation


class FirstWindow(Screen):
    @staticmethod
    def close_app():
        App.get_running_app().stop()


class SecondWindow(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)


class About(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.label_text = "[size=35][color=#FFFF00][b]ASLearner[/b][/color][/size] is an app designed to help the " \
                          "people who cannot speak and the people that desire to learn the sign language; the app " \
                          "contains 3 activities:\n\n" \
                          "[size=33][color=#FFFF00][i]Live Translation[/i][/color][/size]:\nusing this activity, " \
                          "you can receive the translated English text corresponding to the letter the non-hearing " \
                          "person marks at the video camera\n\n" \
                          "[size=33][color=#FFFF00][i]Sign Improver[/i][/color][/size]:\nusing this activity, you can "\
                          "improve your signing knowledge; the program gives a sign for you to mark and waits for " \
                          "you to sign; don't hesitate to reveal a hint in case you feel stuck\n\n" \
                          "[size=33][color=#FFFF00][i]Learn the Basics[/i][/color][/size]:\nit is clearly the " \
                          "starting point for you: you will learn the letters of the ASL alphabet using letter " \
                          "sign models.\n" \
                          "\nThe app adapts the learning experience based on your hand symbols; if you correctly make "\
                          "a gesture while 'Learning the basics', your symbol will be latter used as given models.\n" \
                          "\n[b][color=#FF0000]Make sure[/b][/color] to sign the symbols at the right distance " \
                          "from the video camera and make sure the background doesn't interfere with your " \
                          "learning sessions!" \
                          "\n\n[b][color=#00FF00]Most importantly, have fun learning! :) [/b][/color]"


class SignImprover(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.animation = None
        self.start_time = 0
        self.end_time = 0
        GlobalShared.state_sign_improver = False
        self.to_predict = None
        self.prediction = None
        self.frame = None

        self.preprocessing_object = GlobalShared.preprocessing_object

        new_data, texture = self.process_video_input()
        prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]
        Clock.schedule_interval(self.update_window, 1.0 / 30.0)

    def update_window(self, *args):
        if GlobalShared.sign_improver_index == 0 and GlobalShared.state_sign_improver:
            self.generate_next_learning_symbol()
            self.hide_hint()
        elif GlobalShared.state_sign_improver:
            if self.end_time - self.start_time > 2:
                new_data, texture = self.process_video_input()
                self.update_camera_input(texture)
                self.update_incoming_message(new_data)
                if self.prediction == self.to_predict:
                    self.generate_next_learning_symbol()
                    self.hide_hint()
            self.end_time = perf_counter()

    def process_video_input(self):
        return_value, self.frame = GlobalShared.camera.read()
        display_frame = cv2.flip(self.frame, -1)

        buffer = bytes(display_frame)
        texture = Texture.create(size=(self.frame.shape[1], self.frame.shape[0]), colorfmt='bgr')
        texture.blit_buffer(buffer, colorfmt='bgr', bufferfmt='ubyte')
        data = cv2.resize(self.frame, (224, 224))
        data = np.reshape(data, (-1, 224, 224, 3))

        new_data = self.preprocessing_object.preprocess_input(data)
        return new_data, texture

    def update_incoming_message(self, new_data):
        self.prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]
        if self.prediction == self.to_predict:
            GlobalShared.live_sign_improver_text = f"[size=30]You are signing: " \
                                                   f"[size=40][color=#00FF00][b]{self.prediction}[/b][/color]"
        else:
            GlobalShared.live_sign_improver_text = f"[size=30]You are signing: " \
                                                   f"[size=40][color=#FF0000][b]{self.prediction}[/b][/color]"
        self.ids["yourText"].text = GlobalShared.live_sign_improver_text
        self.ids["SignImproverText"].text = f"[size=30][color=#000000]You have to sign: [/color]" \
                                            f"[size=40][color=#FFFF00][b]{self.to_predict}[/b][/color]"

    def update_camera_input(self, texture):
        self.ids["yourImage"].texture = texture

    def generate_next_learning_symbol(self):
        GlobalShared.sign_improver_index = random.randint(0, 25)
        self.to_predict = GlobalShared.label_key.get(GlobalShared.sign_improver_index)

    def reveal_hint(self):
        self.ids["SignImproverImage"].background_normal = f"_personalization_1/{self.to_predict}.jpg"
        self.ids["SignImproverImage"].text = ""

    def hide_hint(self):
        self.ids["SignImproverImage"].background_normal = ""
        self.ids["SignImproverImage"].text = "Type to reveal hint!"
        self.ids["SignImproverImage"].background_color = (0.5, 0.5, 0.5, 1)

    @staticmethod
    def animate_reveal(widget, *args):
        animation = Animation(background_color=(1, 1, 1, 1), duration=.3)
        animation.start(widget)


class LearnTheBasics(Screen):
    camera_texture = None

    GlobalShared.learning_index = 0
    prediction = ""

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.start_time = 0
        self.end_time = 0
        GlobalShared.state_learn_the_basics = False
        self.to_predict = None
        self.frame = None

        self.preprocessing_object = GlobalShared.preprocessing_object

        new_data, texture = self.process_video_input()
        prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]
        Clock.schedule_interval(self.update_window, 1.0 / 30.0)

    def update_window(self, *args):
        if GlobalShared.learning_index == 0 and GlobalShared.state_learn_the_basics:
            self.generate_next_learning_symbol()
            self.update_signing_symbol()
        elif GlobalShared.state_learn_the_basics:
            if self.end_time - self.start_time > 2:
                new_data, texture = self.process_video_input()
                self.update_camera_input(texture)
                self.update_incoming_message(new_data)
                if self.prediction == self.to_predict:
                    self.generate_next_learning_symbol()
                    self.update_signing_symbol()
                    self.save_data()
            self.end_time = perf_counter()

    def save_data(self):
        image_to_save = cv2.resize(self.frame, (200, 200))
        cv2.imwrite(str(f"_personalization_1/{self.prediction}.jpg"), image_to_save)

    def process_video_input(self):
        return_value, self.frame = GlobalShared.camera.read()
        display_frame = cv2.flip(self.frame, -1)

        buffer = bytes(display_frame)
        texture = Texture.create(size=(self.frame.shape[1], self.frame.shape[0]), colorfmt='bgr')
        texture.blit_buffer(buffer, colorfmt='bgr', bufferfmt='ubyte')
        data = cv2.resize(self.frame, (224, 224))
        data = np.reshape(data, (-1, 224, 224, 3))

        new_data = self.preprocessing_object.preprocess_input(data)
        return new_data, texture

    def update_incoming_message(self, new_data):
        self.prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]
        self.ids["toSignText"].text = f"[size=30][color=#000000]You have to sign: [/color]" \
                                      f"[size=40][color=#FFFF00][b]{self.to_predict}[/b][/color]"

        if self.prediction == self.to_predict:
            GlobalShared.live_learning_basics_text = f"[size=30]You are signing: " \
                                                     f"[size=40][color=#00FF00][b]{self.prediction}[/color][/b]"
        else:
            GlobalShared.live_learning_basics_text = f"[size=30]You are signing: " \
                                                     f"[size=40][color=#FF0000][b]{self.prediction}[/color][/b]"

        self.ids["yourSignText"].text = GlobalShared.live_learning_basics_text

    def update_camera_input(self, texture):
        self.ids["yourSignImage"].texture = texture

    def generate_next_learning_symbol(self):
        GlobalShared.learning_index = (GlobalShared.learning_index + 1) % 26
        self.to_predict = GlobalShared.label_key.get(GlobalShared.learning_index)

    def update_signing_symbol(self):
        self.ids["toSignImage"].source = f"_personalization_1/{self.to_predict}.jpg"


class LiveTranslation(Screen):
    camera_texture = None
    camera_size = (360, 640, 3)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.start_time = 0
        self.end_time = 0
        self.frame = None
        GlobalShared.state_live_translation = False
        self.preprocessing_object = GlobalShared.preprocessing_object

        new_data, texture = self.process_video_input()
        prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]

        Clock.schedule_interval(self.update_camera, 1.0 / 30.0)

    def update_camera(self, *args):
        if GlobalShared.state_live_translation:
            new_data, texture = self.process_video_input()
            self.update_camera_input(texture)
            if (self.end_time - self.start_time) > 1:
                self.update_incoming_message(new_data)
                self.start_time = self.end_time

            self.end_time = perf_counter()

    def process_video_input(self):
        return_value, self.frame = GlobalShared.camera.read()
        display_frame = cv2.flip(self.frame, -1)

        buffer = bytes(display_frame)
        texture = Texture.create(size=(self.frame.shape[1], self.frame.shape[0]), colorfmt='bgr')
        texture.blit_buffer(buffer, colorfmt='bgr', bufferfmt='ubyte')
        data = cv2.resize(self.frame, (224, 224))
        data = np.reshape(data, (-1, 224, 224, 3))

        new_data = self.preprocessing_object.preprocess_input(data)
        return new_data, texture

    def update_incoming_message(self, new_data):
        prediction = GlobalShared.label_key[np.argmax(GlobalShared.model.predict(new_data), axis=1)[0]]
        GlobalShared.live_text_translated += prediction
        self.ids["prediction"].text = GlobalShared.live_text_translated

    def update_camera_input(self, texture):
        self.ids["camera"].texture = texture

    @staticmethod
    def close_app():
        App.get_running_app().stop()


class WindowManager(ScreenManager):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)


class ASLearner(App):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def build(self):
        kv = Builder.load_file('layout.kv')
        Window.size = (360, 640)
        Window.clearcolor = (21 / 255, 93 / 255, 39 / 255, 1)
        return kv


if __name__ == "__main__":
    ASLearner().run()
