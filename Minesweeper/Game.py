import random
import pygame
from typing import Union, List


class Game:

    def __init__(self, resolution):
        """
        Constructor of the class; declares and initializes all the variables needed for this class
        :param resolution: Union[int, int] - the resolution of the screen, in pixels (width X height)
        """
        self.features_minus_buttons = []
        self.features_plus_buttons = []
        self.features_rectangle = []
        pygame.init()
        pygame.font.init()

        self.features = [10, 10, 10, 90]
        directions = [[-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1]]
        self.is_running = True
        self.in_game = True
        self.in_menu = False

        self.button_font = pygame.font.Font('Fonts/Raleway-Bold.ttf', 32)

        self.plus_image = pygame.image.load("Images/Plus.jpg")
        self.minus_image = pygame.image.load("Images/Minus.jpg")

        self.clock = pygame.time.Clock()
        self.screen = pygame.display.set_mode((resolution[0], resolution[1]))
        self.mouse_cursor = pygame.mouse.get_pos()
        self.position_to_draw = [10, 10, 40]
        self.is_click = 0

        self.resolution = resolution
        self.highlight_color = (152, 136, 100)
        self.outline_color = (255, 255, 255)
        self.screen_color = (106, 0, 78)
        self.piece_color = (234, 189, 122)
        self.selected_color = (248, 255, 82)
        self.bomb_color = (214, 40, 40)
        self.flag_color = (73, 41, 5)
        self.text_color = (128, 128, 80)

        self.number_colors = [(3, 0, 251), (5, 126, 3), (254, 1, 2), (1, 1, 128),
                              (125, 1, 2), (2, 127, 127), (255, 255, 0), (0, 255, 255)]

        self.line_number = self.features[0]
        self.column_number = self.features[1]
        self.bomb_number = self.features[2]
        self.game_timer = self.features[3]
        self.passed_time = 0

        self.marked_bombs = []
        self.guessed_bombs = 0
        self.total_passed_tiles = 0

        self.position_to_draw[2] = min(520 // self.line_number, 520 // self.column_number)
        self.position_to_draw[0] = (resolution[1] - self.line_number * self.position_to_draw[2]) / 2
        self.position_to_draw[1] = (resolution[0] - self.column_number * self.position_to_draw[2]) / 2
        self.number_font = pygame.font.Font('Fonts/Raleway-Bold.ttf',
                                            int(480 / max(self.line_number, self.column_number)))

        pygame.display.set_caption("Py: Minesweeper")
        self.board = []
        self.passed_tiles = []
        for i in range(0, self.line_number):
            self.board.append([])
            self.passed_tiles.append([])
            for j in range(0, self.column_number):
                self.board[i].append(0)
                self.passed_tiles[i].append(0)

        bomb_positions = []
        for i in range(0, self.line_number):
            for j in range(0, self.column_number):
                bomb_positions.append([i, j])

        self.bombs_on_board = random.sample(bomb_positions, self.bomb_number)

        for bomb_position in self.bombs_on_board:
            self.board[bomb_position[0]][bomb_position[1]] = -1
            for direction in directions:
                line = bomb_position[0] + direction[0]
                column = bomb_position[1] + direction[1]
                if 0 <= line < self.line_number and 0 <= column < self.column_number \
                        and self.board[line][column] >= 0:
                    self.board[bomb_position[0] + direction[0]][bomb_position[1] + direction[1]] += 1
                    self.passed_tiles[bomb_position[0] + direction[0]][bomb_position[1] + direction[1]] = 0

    def initialize(self):
        """
        The same as the constructor; usage for the "Restart" button
        """
        self.number_font = pygame.font.Font('Fonts/Raleway-Bold.ttf',
                                            int(480 / max(self.line_number, self.column_number)))
        directions = [[-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1]]
        self.is_running = True
        self.in_game = True
        self.in_menu = False

        self.is_click = 0

        self.line_number = self.features[0]
        self.column_number = self.features[1]
        self.bomb_number = self.features[2]
        self.game_timer = self.features[3]
        self.passed_time = 0

        self.marked_bombs = []
        self.guessed_bombs = 0
        self.total_passed_tiles = 0

        self.position_to_draw[2] = min(int(500 / self.line_number), int(500 / self.column_number))
        self.position_to_draw[0] = (self.resolution[1] - self.line_number * self.position_to_draw[2]) / 2
        self.position_to_draw[1] = (self.resolution[0] - self.column_number * self.position_to_draw[2]) / 2

        self.board = []
        self.passed_tiles = []
        for i in range(0, self.line_number):
            self.board.append([])
            self.passed_tiles.append([])
            for j in range(0, self.column_number):
                self.board[i].append(int(0))
                self.passed_tiles[i].append(int(0))

        bomb_positions = []
        for i in range(0, self.line_number):
            for j in range(0, self.column_number):
                bomb_positions.append([i, j])

        self.bombs_on_board = random.sample(bomb_positions, self.bomb_number)

        for bomb_position in self.bombs_on_board:
            self.board[bomb_position[0]][bomb_position[1]] = -1
            for direction in directions:
                line = bomb_position[0] + direction[0]
                column = bomb_position[1] + direction[1]
                if 0 <= line < self.line_number \
                        and 0 <= column < self.column_number \
                        and self.board[line][column] >= 0:
                    self.board[bomb_position[0] + direction[0]][bomb_position[1] + direction[1]] += 1
                    self.passed_tiles[bomb_position[0] + direction[0]][bomb_position[1] + direction[1]] = 0

    def draw_game(self):
        """
        Procedure used for drawing the game;
        firstly, we have to create the font using the board dimensions
        the board itself is drawn taking in account multiple situations:
        - if a cell has been discovered (= 1)
            and it is a bomb on the board, we have to draw a bomb
            else it is a normal cell that has been discovered, so we draw it according to its number
        - if a cell has not been discovered (= 0)
            and the game is still running and the mouse cursor is over a cell:
                we draw it as a highlighted cell
            else we draw it as a normal board cell
        - if a cell has been marked with a flag (= 2)
                we draw it as a flag cell
        """
        self.number_font = pygame.font.Font('Fonts/Raleway-Bold.ttf',
                                            int(480 / max(self.line_number, self.column_number)))
        line_number = len(self.board)
        column_number = len(self.board[0])

        for i in range(0, line_number):
            for j in range(0, column_number):
                if self.passed_tiles[i][j] == 1:
                    if self.board[i][j] == -1:
                        self.draw_rectangle([i, j], self.bomb_color, 3)
                    else:
                        self.draw_rectangle([i, j], self.highlight_color, 3)
                        if self.board[i][j] > 0:
                            self.draw_number_on_cell([i, j], self.number_colors[self.board[i][j] - 1], 3)
                elif self.passed_tiles[i][j] == 0:
                    if self.is_cell_highlighted([i, j]) and self.is_running:
                        self.draw_rectangle([i, j], self.selected_color, 3)
                    else:
                        self.draw_rectangle([i, j], self.piece_color, 3)
                elif self.passed_tiles[i][j] == 2:
                    self.draw_rectangle([i, j], self.flag_color, 3)
                elif self.passed_tiles[i][j] == 2:
                    self.draw_rectangle([i, j], self.flag_color, 3)

    def draw_number_on_cell(self, position, color, outline=0):
        """
        Procedure used for drawing a centered number on a discovered cell
        :param position: List[int, int, int] - List[0] - the y coordinate on screen of the top left corner of the board
                                               List[1] - the x coordinate on screen of the top left corner of the board
                                               List[2] - the dimension of the cell
        :param color: the color used for drawing the cell
        :param outline: the dimension of the margin of the cell - in pixels
        """
        number = self.number_font.render(str(self.board[position[0]][position[1]]), True, color)
        number_rectangle = number.get_rect()
        text_position_x = self.position_to_draw[1] + self.position_to_draw[2] * position[1] - outline
        text_position_y = self.position_to_draw[0] + self.position_to_draw[2] * position[0] - outline
        text_position_x += (self.position_to_draw[2] - number_rectangle.width) / 2
        text_position_y += (self.position_to_draw[2] - number_rectangle.height) / 2
        self.screen.blit(number, (text_position_x, text_position_y))

    def draw_rectangle(self, position, color, outline=0):
        """
        Procedure used for drawing a cell of the board - with outline color
        :param position: List[int, int, int] - List[0] - the y coordinate on screen of the top left corner of the board
                                               List[1] - the x coordinate on screen of the top left corner of the board
                                               List[2] - the dimension of the cell
        :param color: the color used for drawing the cell
        :param outline: the dimension of the margin of the cell - in pixels
        """
        if outline > 0:
            pygame.draw.rect(self.screen,
                             self.outline_color,
                             (self.position_to_draw[1] + self.position_to_draw[2] * position[1] - outline,
                              self.position_to_draw[0] + self.position_to_draw[2] * position[0] - outline,
                              self.position_to_draw[2] + 2 * outline, self.position_to_draw[2] + 2 * outline))

            pygame.draw.rect(self.screen,
                             color,
                             (self.position_to_draw[1] + self.position_to_draw[2] * position[1],
                              self.position_to_draw[0] + self.position_to_draw[2] * position[0],
                              self.position_to_draw[2], self.position_to_draw[2]))
        else:
            pygame.draw.rect(self.screen,
                             color,
                             (self.position_to_draw[1] + self.position_to_draw[2] * position[1] - outline,
                              self.position_to_draw[0] + self.position_to_draw[2] * position[0] - outline,
                              self.position_to_draw[2], self.position_to_draw[2]))

    def is_cell_highlighted(self, position):
        """
        Function used to determine whether the cursor of the mouse is over a cell
            - we take the mouse cursor position
            - we convert it in board cell coordinates
            - we check if the coordinates are inside the board
                if it is true, we check and return if the mouse cursor position is inside the respective cell
        :param position: List[int, int, int] - List[0] - the y coordinate on screen of the top left corner of the board
                                               List[1] - the x coordinate on screen of the top left corner of the board
                                               List[2] - the dimension of the cell
        :return: True if the mouse position is over a board cell
                 False, otherwise
        """
        line_number = len(self.board)
        column_number = len(self.board[0])
        mouse_cursor = pygame.mouse.get_pos()
        board_cell = self.get_cell()
        x_start = self.position_to_draw[1] + self.position_to_draw[2] * position[1]
        y_start = self.position_to_draw[0] + self.position_to_draw[2] * position[0]
        if not (0 <= board_cell[0] < line_number and 0 <= board_cell[1] < column_number):
            return False

        if x_start < mouse_cursor[0] < x_start + self.position_to_draw[2] \
                and y_start < mouse_cursor[1] < y_start + self.position_to_draw[2]:
            return True
        return False

    def is_rectangle_highlighted(self, button_rectangle):
        """
        Function used to check if the mouse cursor position is over a button
        :param button_rectangle: List[int, int, int int] - List[0] - the x coordinate of the top left corner of a button
                                                         - List[1] - the y coordinate of the top left corner of a button
                                                         - List[2] - the width of the button
                                                         - List[3] - the height of the button
        :return: True, if the mouse cursor position is over a button
                 False, otherwise
        """
        self.mouse_cursor = pygame.mouse.get_pos()
        if button_rectangle[0] < self.mouse_cursor[0] < button_rectangle[0] + button_rectangle[2] \
                and button_rectangle[1] < self.mouse_cursor[1] < button_rectangle[1] + button_rectangle[3]:
            return True
        return False

    def get_cell(self):
        """
        Function used for converting mouse cursor screen coordinates in board cell coordinates
        :return: Union[int, int] - the board cell coordinates of the mouse cursor position
        """
        position_on_screen = pygame.mouse.get_pos()
        column = (position_on_screen[0] - self.position_to_draw[1]) / self.position_to_draw[2]
        line = (position_on_screen[1] - self.position_to_draw[0]) / self.position_to_draw[2]
        return [int(line), int(column)]

    def highlight_land(self, position: list[Union[int, int]]):
        """
        Procedure used for highlighting the board after we have clicked a board cell
               - we initialize the queue with the board cell position we clicked
               - firstly, we mark the cell as discovered if it was not discovered in the past and we count it
               - if the cell is a bomb or a number, we have our job done: we have to discover no more cells
               - if the cell is empty, we have to show all the empty cells
               and the cells with numbers in the neighbourhood
                    - for this task, we apply Lee Algorithm:
                        - we take an element from the queue
                        - we check all its neighbours:
                            - if the neighbour is an undiscovered empty cell, we mark it as discovered,
                                we count it and we add it to the queue
                            - if the neighbour is an undiscovered number cell, we mark it as discovered
                                and we count it
        :param position: List[int, int] - the board cell coordinates
        """
        directions = [[-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1]]
        queue = List[Union[int, int]]
        queue = [[position[0], position[1]]]

        if self.passed_tiles[position[0]][position[1]] == 0:
            self.passed_tiles[position[0]][position[1]] = 1
            self.total_passed_tiles += 1
        if self.board[position[0]][position[1]] == -1:
            return
        if self.board[position[0]][position[1]] > 0:
            return
        line_number = len(self.board)
        column_number = len(self.board[0])
        index = 0
        n = len(queue)
        while index < n:
            element = queue[index]
            for direction in directions:
                new_line = int(element[0] + direction[0])
                new_column = int(element[1] + direction[1])
                if line_number > new_line >= 0 and 0 <= new_column < column_number:
                    if self.passed_tiles[new_line][new_column] == 0:
                        self.total_passed_tiles += 1
                        if self.board[new_line][new_column] > 0:
                            self.passed_tiles[new_line][new_column] = 1
                        elif self.board[new_line][new_column] == 0:
                            queue.append([new_line, new_column])
                            self.passed_tiles[new_line][new_column] = 1
            index += 1
            n = len(queue)

    def mark_bomb(self):
        """
        Procedure used for marking a board cell with a flag:
            - if the board cell coordinates of the mouse cursor position are inside the board
                we check if the board cell coordinates determine a bomb and if we have free flags, we mark the cell,
                    we count it as a bomb and we keep in touch with the coordinates of the marked bomb
            - if we try to mark a flag once more, we will remove the flag:
                we mark the cell as undiscovered, we remove the marked cell from the list and we decrease the number
                    of the guesses in case we removed the flag from a bomb cell
        """
        clicked_cell = self.get_cell()
        line_number = len(self.board)
        column_number = len(self.board[0])

        if line_number > clicked_cell[0] >= 0 and column_number > clicked_cell[1] >= 0:
            if self.passed_tiles[clicked_cell[0]][clicked_cell[1]] == 0 \
                    and len(self.marked_bombs) < self.bomb_number:
                self.passed_tiles[clicked_cell[0]][clicked_cell[1]] = 2
                self.marked_bombs.append(clicked_cell)
                if self.board[clicked_cell[0]][clicked_cell[1]] == -1:
                    self.guessed_bombs += 1
            elif self.passed_tiles[clicked_cell[0]][clicked_cell[1]] == 2:
                self.passed_tiles[clicked_cell[0]][clicked_cell[1]] = 0
                index_of_element = self.marked_bombs.index(clicked_cell)
                self.marked_bombs.pop(index_of_element)
                if self.board[clicked_cell[0]][clicked_cell[1]] == -1:
                    self.guessed_bombs -= 1

    def get_completion_state(self):
        """
        Function used to determine if the game has finished or not;
            one of the finishing conditions is to have discovered all the cells and to mark all the bombs;
            another one is regarding the time limit; if the passed time is greater than the timer, then the game ends
        :return: 1 if the game reached its finishing state
                 0 if the game has to continue
        """
        total_lines = self.features[0]
        total_columns = self.features[1]
        if self.passed_time > self.game_timer:
            return 1
        if self.total_passed_tiles + self.guessed_bombs == total_lines * total_columns:
            return 1
        else:
            return 0

    def highlight_bombs(self):
        """
        Procedure used for highlighting the bombs if the player clicked a bomb cell
        """
        for bomb_position in self.bombs_on_board:
            self.passed_tiles[bomb_position[0]][bomb_position[1]] = 1

    def play_game(self):
        """
        Procedure used for the game logic;
            Firstly, we initialise the parameters
            After that, we enter the game loop
            We check for events and we call the respective functions for every situation we encounter
        """
        self.initialize()
        time_start = pygame.time.get_ticks()
        while self.in_game:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.is_running = False
                    self.in_game = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:
                        self.is_click = 1
                        mouse_cell_position = self.get_cell()
                        if self.is_cell_highlighted(mouse_cell_position) and self.is_running:
                            if self.board[mouse_cell_position[0]][mouse_cell_position[1]] != -1:
                                self.highlight_land(self.get_cell())
                            elif self.is_running and not self.get_completion_state():
                                self.highlight_bombs()
                                self.is_running = False
                    elif event.button == 3:
                        self.mark_bomb()
                elif event.type == pygame.MOUSEBUTTONUP:
                    if event.button == 1:
                        self.is_click = 0
            time_end = pygame.time.get_ticks()
            delta_time = (time_end - time_start) / 1000
            if self.is_running and not self.get_completion_state():
                self.passed_time += delta_time
            if self.passed_time > self.game_timer:
                self.is_running = False
            self.screen.fill(self.screen_color)
            self.draw_game()
            if self.is_running and not self.get_completion_state() and self.passed_time < self.game_timer:
                self.draw_time()
            self.draw_end_screen()
            pygame.display.update()
            self.clock.tick(60)
            time_start = time_end

    def draw_end_screen(self):
        """
        Procedure used for drawing the end-screen
        Here we determine if the player won or lost and we draw on the screen the respective message
        After that, we draw the "Restart" and "Exit" buttons
        If we click "Restart", we can play another game with the same configurations for the board, but with different
            positions for the bombs
        If we click "Exit", we close the game
        """
        message = 0
        if (not self.is_running and not self.get_completion_state()) or \
                (self.game_timer < self.passed_time and not self.is_running):
            message = "YOU LOST!"
        elif self.is_running and self.get_completion_state():
            message = "YOU WON!"
        if message:
            message_on_screen = self.button_font.render(message, True, self.outline_color)
            message_rectangle = message_on_screen.get_rect()
            text_position_x = (self.resolution[0] - message_rectangle.width) / 2
            text_position_y = (self.position_to_draw[0] - message_rectangle.height) / 2
            self.screen.blit(message_on_screen, (text_position_x, text_position_y))

            restart = self.button_font.render("Restart", True, self.outline_color, self.selected_color)
            restart_rectangle = message_on_screen.get_rect()
            text_position_x = 10
            text_position_y = self.resolution[1] - message_rectangle.height - 10
            restart_rectangle = [text_position_x,
                                 text_position_y,
                                 restart_rectangle.width,
                                 restart_rectangle.height]

            if not self.is_rectangle_highlighted(restart_rectangle):
                restart = self.button_font.render("Restart", True, self.text_color, self.outline_color)
                self.screen.blit(restart, (text_position_x, text_position_y))
            else:
                restart = self.button_font.render("Restart", True, self.outline_color, self.text_color)
                self.screen.blit(restart, (text_position_x, text_position_y))
                if self.is_click:
                    self.initialize()

            exit_surface = self.button_font.render("Exit", True, self.outline_color, self.selected_color)
            exit_rectangle = exit_surface.get_rect()
            text_position_x = self.resolution[0] - exit_rectangle.width - 10
            text_position_y = self.resolution[1] - exit_rectangle.height - 10
            exit_rectangle = [text_position_x,
                              text_position_y,
                              exit_rectangle.width,
                              exit_rectangle.height]
            if not self.is_rectangle_highlighted(exit_rectangle):
                exit_surface = self.button_font.render("Exit", True, self.text_color, self.outline_color)
                self.screen.blit(exit_surface, (text_position_x, text_position_y))
            else:
                exit_surface = self.button_font.render("Exit", True, self.outline_color, self.text_color)
                self.screen.blit(exit_surface, (text_position_x, text_position_y))
                if self.is_click:
                    self.in_game = False

    def draw_time(self):
        """
        Procedure used for drawing the remaining time during the game phase
        """
        time_surface = self.button_font.render(f"Time left: {int(self.game_timer - self.passed_time)}s",
                                               True,
                                               self.outline_color)
        time_rectangle = time_surface.get_rect()
        text_position_x = (self.resolution[0] - time_rectangle.width) / 2
        text_position_y = 0
        self.screen.blit(time_surface, (text_position_x, text_position_y))

    def draw_menu(self):
        """
        Procedure used for drawing the main-menu with the parameters that can be configured for the game
        """
        lines_surface = self.button_font.render("Lines: ", True, self.outline_color)
        lines_rectangle = lines_surface.get_rect()
        text_position_x = self.resolution[0] / 5 - lines_rectangle.width + 200
        text_position_y = self.resolution[1] / 5 - lines_rectangle.height - 10
        self.features_rectangle.append([text_position_x, text_position_y, lines_rectangle.w, lines_rectangle.h])
        self.screen.blit(lines_surface, (text_position_x, text_position_y))

        columns_surface = self.button_font.render("Columns: ", True, self.outline_color)
        columns_rectangle = columns_surface.get_rect()
        text_position_x = self.resolution[0] / 5 - columns_rectangle.width + 200
        text_position_y = 2 * self.resolution[1] / 5 - columns_rectangle.height - 10
        self.features_rectangle.append([text_position_x, text_position_y, columns_rectangle.w, columns_rectangle.h])
        self.screen.blit(columns_surface, (text_position_x, text_position_y))

        bombs_surface = self.button_font.render("Bombs: ", True, self.outline_color)
        bombs_rectangle = bombs_surface.get_rect()
        text_position_x = self.resolution[0] / 5 - bombs_rectangle.width + 200
        text_position_y = 3 * self.resolution[1] / 5 - bombs_rectangle.height - 10
        self.features_rectangle.append([text_position_x, text_position_y, bombs_rectangle.w, bombs_rectangle.h])
        self.screen.blit(bombs_surface, (text_position_x, text_position_y))

        time_surface = self.button_font.render("Time: ", True, self.outline_color)
        time_rectangle = time_surface.get_rect()
        text_position_x = self.resolution[0] / 5 - time_rectangle.width + 200
        text_position_y = 4 * self.resolution[1] / 5 - time_rectangle.height - 10
        self.features_rectangle.append([text_position_x, text_position_y, time_rectangle.w, time_rectangle.h])
        self.screen.blit(time_surface, (text_position_x, text_position_y))

        play_surface = self.button_font.render("Play", True, self.text_color, self.outline_color)
        play_rectangle = play_surface.get_rect()
        text_position_x = self.resolution[0] / 2 + play_rectangle.width - 110
        text_position_y = self.resolution[1] / 2 + play_rectangle.height + 150
        play_rectangle = [text_position_x,
                          text_position_y,
                          play_rectangle.width,
                          play_rectangle.height]
        if not self.is_rectangle_highlighted(play_rectangle):
            play_surface = self.button_font.render("Play", True, self.text_color, self.outline_color)
            self.screen.blit(play_surface, (text_position_x, text_position_y))
        else:
            play_surface = self.button_font.render("Play", True, self.outline_color, self.text_color)
            self.screen.blit(play_surface, (text_position_x, text_position_y))
            if self.is_click:
                self.in_menu = False

        exit_surface = self.button_font.render("Exit", True, self.outline_color, self.text_color)
        exit_rectangle = exit_surface.get_rect()
        text_position_x = self.resolution[0] / 2 + exit_rectangle.width - 100
        text_position_y = self.resolution[1] / 2 + exit_rectangle.height + 200
        exit_rectangle = [text_position_x,
                          text_position_y,
                          exit_rectangle.width,
                          exit_rectangle.height]
        if not self.is_rectangle_highlighted(exit_rectangle):
            exit_surface = self.button_font.render("Exit", True, self.text_color, self.outline_color)
            self.screen.blit(exit_surface, (text_position_x, text_position_y))
        else:
            exit_surface = self.button_font.render("Exit", True, self.outline_color, self.text_color)
            self.screen.blit(exit_surface, (text_position_x, text_position_y))
            if self.is_click:
                self.in_menu = False
                self.in_game = False
                self.is_running = False

        for j in range(0, 4):
            surface = self.button_font.render(str(self.features[j]), True, self.outline_color)
            rectangle = surface.get_rect()
            text_position_x = self.features_rectangle[j][0] + self.features_rectangle[j][2] + 10
            text_position_y = self.features_rectangle[j][1]
            self.features_plus_buttons.append([text_position_x + rectangle.w + 30,
                                               text_position_y,
                                               rectangle.w,
                                               rectangle.h])
            self.features_minus_buttons.append([self.features_plus_buttons[j][0] +
                                                self.features_plus_buttons[j][2] + 10,
                                                text_position_y,
                                                rectangle.w,
                                                rectangle.h])

            self.screen.blit(self.plus_image, (self.features_plus_buttons[j][0],
                                               self.features_plus_buttons[j][1]))
            self.screen.blit(self.minus_image, (self.features_minus_buttons[j][0],
                                                self.features_minus_buttons[j][1]))

            self.screen.blit(surface, (text_position_x, text_position_y))

    def main_menu(self):
        """
        Procedure used for the main-menu logic; listening for the events and calling for the procedures according to
        the situations we encounter
        """
        time_start = pygame.time.get_ticks()
        self.in_menu = True
        passed_time = 0
        while self.in_menu:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.in_menu = False
                    self.is_running = False
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:
                        self.is_click = True
                elif event.type == pygame.MOUSEBUTTONUP:
                    if event.button == 1:
                        self.is_click = False
            time_end = pygame.time.get_ticks()
            delta_time = (time_end - time_start) / 1000

            feature_size = len(self.features)
            timer = 0.1
            for i in range(0, feature_size):
                if self.is_click and passed_time > timer:
                    if self.is_rectangle_highlighted(self.features_plus_buttons[i]):
                        self.features[i] += 1
                        passed_time = 0
                        if self.features[i] > 30 and i < 3:
                            self.features[i] = 10
                    elif self.is_rectangle_highlighted(self.features_minus_buttons[i]):
                        self.features[i] -= 1
                        if self.features[i] < 10:
                            self.features[i] = 10
                        passed_time = 0

            if self.features[2] > self.features[0] * self.features[1]:
                self.features[2] = self.features[0] * self.features[1]
            if self.features[3] < 90:
                self.features[3] = 90
            passed_time += delta_time
            time_start = time_end
            self.screen.fill(self.screen_color)
            self.draw_menu()
            pygame.display.update()
            self.clock.tick(60)
        if self.in_game and self.is_running:
            self.play_game()
