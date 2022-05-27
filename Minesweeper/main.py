import Game


def start_game():
    minesweeper = Game.Game([800, 600])
    minesweeper.main_menu()


if __name__ == "__main__":
    start_game()
